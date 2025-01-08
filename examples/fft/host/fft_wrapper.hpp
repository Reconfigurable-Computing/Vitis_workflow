#include "event_timer.hpp"
#include "global.h"
#include "kernel/fft.h"

// fft内核的opencl wrapper
void fft_wrapper(vec_t X_R, vec_t X_I, vec_t& OUT_R, vec_t& OUT_I, std::string xclbin) {
    EventTimer et;
    cl_int err;
    cl::Context context;
    cl::CommandQueue q;
    // std::vector<cl::Kernel> krnls(NUM_KERNEL);
    // cl::Kernel krnl, krnl_read, krnl_write;
    cl::Kernel krnl_fft;

    // OPENCL HOST CODE AREA START
    et.add("OpenCL Initialization");
    auto devices = xcl::get_xil_devices();
    auto fileBuf = xcl::read_binary_file(xclbin);
    cl::Program::Binaries bins{{fileBuf.data(), fileBuf.size()}};
    bool valid_device = false;
    for (unsigned int i = 0; i < devices.size(); i++) {
        auto device = devices[i];
        // Creating Context and Command Queue for selected Device
        OCL_CHECK(err, context = cl::Context(device, nullptr, nullptr, nullptr, &err));
        OCL_CHECK(err, q = cl::CommandQueue(context, device,
                                            CL_QUEUE_OUT_OF_ORDER_EXEC_MODE_ENABLE | CL_QUEUE_PROFILING_ENABLE,
                                            &err));

        std::cout << "Trying to program device[" << i << "]: " << device.getInfo<CL_DEVICE_NAME>() << std::endl;
        cl::Program program(context, {device}, bins, nullptr, &err);
        if (err != CL_SUCCESS) {
            std::cout << "Failed to program device[" << i << "] with xclbin file!\n";
        } else {
            std::cout << "Device[" << i << "]: program successful!\n";

            // Creating Kernel object using Compute unit names
            OCL_CHECK(err, krnl_fft = cl::Kernel(program, "fft", &err));
            valid_device = true;
            break; // we break because we found a valid device
        }
    }
    if (!valid_device) {
        std::cout << "Failed to program any device found, exit!\n";
        exit(EXIT_FAILURE);
    }
    et.finish();

    /* Host mem flags */
    cl_mem_ext_ptr_t X_R_ext;
    cl_mem_ext_ptr_t X_I_ext;
    cl_mem_ext_ptr_t OUT_R_ext;
    cl_mem_ext_ptr_t OUT_I_ext;

    X_R_ext.obj = X_R.data();
    X_R_ext.param = 0;
    X_R_ext.flags = HBM[0];

    X_I_ext.obj = X_I.data();
    X_I_ext.param = 0;
    X_I_ext.flags = HBM[1];

    OUT_R_ext.obj = OUT_R.data();
    OUT_R_ext.param = 0;
    OUT_R_ext.flags = HBM[2];

    OUT_I_ext.obj = OUT_I.data();
    OUT_I_ext.param = 0;
    OUT_I_ext.flags = HBM[3];

    cl::Buffer X_R_buf;
    cl::Buffer X_I_buf;
    cl::Buffer OUT_R_buf;
    cl::Buffer OUT_I_buf;

    et.add("Map host buffers to OpenCL buffers");
    OCL_CHECK(err, X_R_buf = cl::Buffer(context, CL_MEM_EXT_PTR_XILINX | CL_MEM_USE_HOST_PTR | CL_MEM_READ_WRITE,
                                        sizeof(DTYPE) * X_R.size(), &X_R_ext, &err));

    OCL_CHECK(err, X_I_buf = cl::Buffer(context, CL_MEM_EXT_PTR_XILINX | CL_MEM_USE_HOST_PTR | CL_MEM_READ_WRITE,
                                        sizeof(DTYPE) * X_I.size(), &X_I_ext, &err));

    OCL_CHECK(err, OUT_R_buf = cl::Buffer(context, CL_MEM_EXT_PTR_XILINX | CL_MEM_USE_HOST_PTR | CL_MEM_READ_WRITE,
                                          sizeof(DTYPE) * OUT_R.size(), &OUT_R_ext, &err));

    OCL_CHECK(err, OUT_I_buf = cl::Buffer(context, CL_MEM_EXT_PTR_XILINX | CL_MEM_USE_HOST_PTR | CL_MEM_READ_WRITE,
                                          sizeof(DTYPE) * OUT_I.size(), &OUT_I_ext, &err));
    et.finish();

    et.add("Set kernel arguments");
    OCL_CHECK(err, err = krnl_fft.setArg(0, X_R_buf));
    OCL_CHECK(err, err = krnl_fft.setArg(1, X_I_buf));
    OCL_CHECK(err, err = krnl_fft.setArg(2, OUT_R_buf));
    OCL_CHECK(err, err = krnl_fft.setArg(3, OUT_I_buf));

    et.add("Memory object migration enqueue");
    OCL_CHECK(err, err = q.enqueueMigrateMemObjects({X_R_buf, X_I_buf}, 0 /* 0 means from host*/));
    q.finish();

    int num_runs = 1000;//执行次数（测得当num-runs=1000左右时达到最大THROUGHPUT=475）
    et.add("OCL Enqueue task and wait for kernel to complete");
    auto t1 = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < num_runs; i++) {
        OCL_CHECK(err, err = q.enqueueTask(krnl_fft));
        q.finish();
    }
    auto t2 = std::chrono::high_resolution_clock::now();

    et.add("Read back computation results");
    // Copy Result from Device Global Memory to Host Local Memory
    OCL_CHECK(err, err = q.enqueueMigrateMemObjects({OUT_R_buf, OUT_I_buf}, CL_MIGRATE_MEM_OBJECT_HOST));
    q.finish();
    et.finish();

    //打印各阶段用时
    et.print();

    //打印kernel吞吐量
    float average_time_in_sec =
        float(std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count()) / 1000000 / num_runs;
    std::cout << "average_time: " << average_time_in_sec * 1000 << " ms" << std::endl;
    double throughput = 1; //表示执行一次长度为SIZE的FFT
    throughput /= average_time_in_sec;
    std::cout << "Compute THROUGHPUT = " << throughput << " /s" << std::endl;
}

// 使用events同步的写法
// void fft_wrapper(vec_t X_R, vec_t X_I, vec_t& OUT_R, vec_t& OUT_I, std::string xclbin) {
//     EventTimer et;
//     cl_int err;
//     cl::Context context;
//     cl::CommandQueue q;
//     // std::vector<cl::Kernel> krnls(NUM_KERNEL);
//     // cl::Kernel krnl, krnl_read, krnl_write;
//     cl::Kernel krnl_fft;

//     // OPENCL HOST CODE AREA START
//     et.add("OpenCL Initialization");
//     auto devices = xcl::get_xil_devices();
//     auto fileBuf = xcl::read_binary_file(xclbin);
//     cl::Program::Binaries bins{{fileBuf.data(), fileBuf.size()}};
//     bool valid_device = false;
//     for (unsigned int i = 0; i < devices.size(); i++) {
//         auto device = devices[i];
//         // Creating Context and Command Queue for selected Device
//         OCL_CHECK(err, context = cl::Context(device, nullptr, nullptr, nullptr, &err));
//         OCL_CHECK(err, q = cl::CommandQueue(context, device,
//                                             CL_QUEUE_OUT_OF_ORDER_EXEC_MODE_ENABLE | CL_QUEUE_PROFILING_ENABLE, &err));

//         std::cout << "Trying to program device[" << i << "]: " << device.getInfo<CL_DEVICE_NAME>() << std::endl;
//         cl::Program program(context, {device}, bins, nullptr, &err);
//         if (err != CL_SUCCESS) {
//             std::cout << "Failed to program device[" << i << "] with xclbin file!\n";
//         } else {
//             std::cout << "Device[" << i << "]: program successful!\n";

//             // Creating Kernel object using Compute unit names
//             OCL_CHECK(err, krnl_fft = cl::Kernel(program, "fft", &err));
//             valid_device = true;
//             break; // we break because we found a valid device
//         }
//     }
//     if (!valid_device) {
//         std::cout << "Failed to program any device found, exit!\n";
//         exit(EXIT_FAILURE);
//     }
//     et.finish();

//     /* Host mem flags */
//     cl_mem_ext_ptr_t X_R_ext;
//     cl_mem_ext_ptr_t X_I_ext;
//     cl_mem_ext_ptr_t OUT_R_ext;
//     cl_mem_ext_ptr_t OUT_I_ext;

//     X_R_ext.obj = X_R.data();
//     X_R_ext.param = 0;
//     X_R_ext.flags = HBM[0];

//     X_I_ext.obj = X_I.data();
//     X_I_ext.param = 0;
//     X_I_ext.flags = HBM[1];

//     OUT_R_ext.obj = OUT_R.data();
//     OUT_R_ext.param = 0;
//     OUT_R_ext.flags = HBM[2];

//     OUT_I_ext.obj = OUT_I.data();
//     OUT_I_ext.param = 0;
//     OUT_I_ext.flags = HBM[3];

//     cl::Buffer X_R_buf;
//     cl::Buffer X_I_buf;
//     cl::Buffer OUT_R_buf;
//     cl::Buffer OUT_I_buf;

//     et.add("Map host buffers to OpenCL buffers");
//     OCL_CHECK(err, X_R_buf = cl::Buffer(context, CL_MEM_EXT_PTR_XILINX | CL_MEM_USE_HOST_PTR | CL_MEM_READ_WRITE,
//                                         sizeof(DTYPE) * X_R.size(), &X_R_ext, &err));

//     OCL_CHECK(err, X_I_buf = cl::Buffer(context, CL_MEM_EXT_PTR_XILINX | CL_MEM_USE_HOST_PTR | CL_MEM_READ_WRITE,
//                                         sizeof(DTYPE) * X_I.size(), &X_I_ext, &err));

//     OCL_CHECK(err, OUT_R_buf = cl::Buffer(context, CL_MEM_EXT_PTR_XILINX | CL_MEM_USE_HOST_PTR | CL_MEM_READ_WRITE,
//                                           sizeof(DTYPE) * OUT_R.size(), &OUT_R_ext, &err));

//     OCL_CHECK(err, OUT_I_buf = cl::Buffer(context, CL_MEM_EXT_PTR_XILINX | CL_MEM_USE_HOST_PTR | CL_MEM_READ_WRITE,
//                                           sizeof(DTYPE) * OUT_I.size(), &OUT_I_ext, &err));
//     et.finish();

//     et.add("Set kernel arguments");
//     OCL_CHECK(err, err = krnl_fft.setArg(0, X_R_buf));
//     OCL_CHECK(err, err = krnl_fft.setArg(1, X_I_buf));
//     OCL_CHECK(err, err = krnl_fft.setArg(2, OUT_R_buf));
//     OCL_CHECK(err, err = krnl_fft.setArg(3, OUT_I_buf));

//     std::vector<cl::Event> events_write(1);
//     std::vector<cl::Event> events_kernel(1);
//     std::vector<cl::Event> events_read(1);

//     et.add("Memory object migration enqueue");
//     OCL_CHECK(
//         err, err = q.enqueueMigrateMemObjects({X_R_buf, X_I_buf}, 0 /* 0 means from host*/, nullptr, &events_write[0]));
//     clWaitForEvents(1, (const cl_event*)&events_write[0]);

//     int num_runs = 1000; //执行次数（测得当num-runs=1000左右时达到最大THROUGHPUT=475）
//     et.add("OCL Enqueue task and wait for kernel to complete");
//     auto t1 = std::chrono::high_resolution_clock::now();
//     for (int i = 0; i < num_runs; i++) {
//         OCL_CHECK(err, err = q.enqueueTask(krnl_fft, &events_write, &events_kernel[0]));
//         clWaitForEvents(1, (const cl_event*)&events_kernel[0]);
//     }
//     auto t2 = std::chrono::high_resolution_clock::now();

//     et.add("Read back computation results");
//     // Copy Result from Device Global Memory to Host Local Memory
//     OCL_CHECK(err, err = q.enqueueMigrateMemObjects({OUT_R_buf, OUT_I_buf}, CL_MIGRATE_MEM_OBJECT_HOST, &events_kernel,
//                                                     &events_read[0]));
//     clWaitForEvents(1, (const cl_event*)&events_read[0]);
//     et.finish();

//     q.finish();

//     //打印各阶段用时
//     et.print();

//     //打印kernel吞吐量
//     float average_time_in_sec =
//         float(std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count()) / 1000000 / num_runs;
//     std::cout << "average_time: " << average_time_in_sec * 1000 << " ms" << std::endl;
//     double throughput = 1; //表示执行一次长度为SIZE的FFT
//     throughput /= average_time_in_sec;
//     std::cout << "Compute THROUGHPUT = " << throughput << " /s" << std::endl;
// }