#include "event_timer.hpp"
#include "global.h"
#include "kernel/vmul.h"

inline void vadd_vmul_wrapper(vec_t in1, vec_t in2, vec_t& res1, vec_t& res2, int LEN, std::string xclbin) {
    EventTimer et;
    cl_int err;
    cl::Context context;
    cl::CommandQueue q;

    // TODO: 添加kernel
    cl::Kernel krnl_vadd;
    cl::Kernel krnl_vmul;

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
                                            CL_QUEUE_OUT_OF_ORDER_EXEC_MODE_ENABLE | CL_QUEUE_PROFILING_ENABLE, &err));

        std::cout << "Trying to program device[" << i << "]: " << device.getInfo<CL_DEVICE_NAME>() << std::endl;
        cl::Program program(context, {device}, bins, nullptr, &err);
        if (err != CL_SUCCESS) {
            std::cout << "Failed to program device[" << i << "] with xclbin file!\n";
        } else {
            std::cout << "Device[" << i << "]: program successful!\n";

            // Creating Kernel object using Compute unit names
            // TODO: 注册kernel
            OCL_CHECK(err, krnl_vadd = cl::Kernel(program, "vadd", &err));
            OCL_CHECK(err, krnl_vmul = cl::Kernel(program, "vmul", &err));
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
    cl_mem_ext_ptr_t in1_ext;
    cl_mem_ext_ptr_t in2_ext;
    cl_mem_ext_ptr_t res1_ext;
    cl_mem_ext_ptr_t res2_ext;

    in1_ext.obj = in1.data();
    in1_ext.param = 0;
    in1_ext.flags = HBM[0];

    in2_ext.obj = in2.data();
    in2_ext.param = 0;
    in2_ext.flags = HBM[0];

    res1_ext.obj = res1.data();
    res1_ext.param = 0;
    res1_ext.flags = HBM[0];

    res2_ext.obj = res2.data();
    res2_ext.param = 0;
    res2_ext.flags = HBM[0];

    cl::Buffer in1_buf;
    cl::Buffer in2_buf;
    cl::Buffer res1_buf;
    cl::Buffer res2_buf;

    et.add("Map host buffers to OpenCL buffers");
    OCL_CHECK(err, in1_buf = cl::Buffer(context, CL_MEM_EXT_PTR_XILINX | CL_MEM_USE_HOST_PTR | CL_MEM_READ_WRITE,
                                        sizeof(dt) * in1.size(), &in1_ext, &err));
    OCL_CHECK(err, in2_buf = cl::Buffer(context, CL_MEM_EXT_PTR_XILINX | CL_MEM_USE_HOST_PTR | CL_MEM_READ_WRITE,
                                        sizeof(dt) * in2.size(), &in2_ext, &err));
    OCL_CHECK(err, res1_buf = cl::Buffer(context, CL_MEM_EXT_PTR_XILINX | CL_MEM_USE_HOST_PTR | CL_MEM_READ_WRITE,
                                         sizeof(dt) * res1.size(), &res1_ext, &err));
    OCL_CHECK(err, res2_buf = cl::Buffer(context, CL_MEM_EXT_PTR_XILINX | CL_MEM_USE_HOST_PTR | CL_MEM_READ_WRITE,
                                         sizeof(dt) * res2.size(), &res2_ext, &err));
    et.finish();

    et.add("Set kernel arguments");
    OCL_CHECK(err, err = krnl_vadd.setArg(0, in1_buf));
    OCL_CHECK(err, err = krnl_vadd.setArg(1, in2_buf));
    OCL_CHECK(err, err = krnl_vadd.setArg(2, res1_buf));
    OCL_CHECK(err, err = krnl_vmul.setArg(0, in1_buf));
    OCL_CHECK(err, err = krnl_vmul.setArg(1, in2_buf));
    OCL_CHECK(err, err = krnl_vmul.setArg(2, res2_buf));

    et.add("Memory object migration enqueue");
    OCL_CHECK(err, err = q.enqueueMigrateMemObjects({in1_buf, in2_buf}, 0 /* 0 means from host*/));
    q.finish();

    int num_runs = 1; //执行次数
    et.add("OCL Enqueue task and wait for kernel to complete");
    auto t1 = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < num_runs; i++) {
        OCL_CHECK(err, err = q.enqueueTask(krnl_vadd));
        OCL_CHECK(err, err = q.enqueueTask(krnl_vmul));
        q.finish();
    }
    auto t2 = std::chrono::high_resolution_clock::now();

    et.add("Read back computation results");
    // Copy Result from Device Global Memory to Host Local Memory
    OCL_CHECK(err, err = q.enqueueMigrateMemObjects({res1_buf, res2_buf}, CL_MIGRATE_MEM_OBJECT_HOST));
    q.finish();
    et.finish();

    //打印各阶段用时
    et.print();

    //打印两个kernel计算吞吐量
    float average_time_in_sec =
        float(std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count()) / 1000000 / num_runs;
    std::cout << "average_time: " << average_time_in_sec * 1000 << " ms" << std::endl;
    double throughput = res1.size() + res2.size(); //处理的数据量
    throughput /= average_time_in_sec;
    std::cout << "Compute THROUGHPUT = " << throughput << " /s" << std::endl;
}