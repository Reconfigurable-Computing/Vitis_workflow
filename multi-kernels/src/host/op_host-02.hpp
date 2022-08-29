#include "event_timer.hpp"
#include "xcl2.hpp"
#include <sys/time.h>

typedef int dt;

void op(dt *in1, dt *in2, dt *res1, dt *res2, int LEN, std::string xclbin){
	EventTimer et;

	cl_int fail;

	//***************************************** Step1 platform related operations
	std::vector<cl::Device> devices = xcl::get_xil_devices();
	cl::Device device = devices[0]; //the device 0

	//***************************************** Step2 Creating Context and Command Queue for selected Device


	et.add("OpenCL Initialization");

	cl::Context context(device, NULL, NULL, NULL, &fail); //initial OpenCL environment

	cl::CommandQueue q(context, device, CL_QUEUE_PROFILING_ENABLE | CL_QUEUE_OUT_OF_ORDER_EXEC_MODE_ENABLE, &fail); //command queue

	cl::Program::Binaries xclBins = xcl::import_binary_file(xclbin); //load the binary file
	devices.resize(1);
	cl::Program program(context, devices, xclBins, NULL, &fail); // pragram the fpga

	cl::Kernel vadd_kernel,vmul_kernel;
	printf("load kernel start\n");
	vadd_kernel = cl::Kernel(program, "vadd_kernel", &fail);//The name "vadd_kernel" should be same as your kernel name .xo
	vmul_kernel = cl::Kernel(program, "vmul_kernel", &fail);
	printf("load kernel finish\n");
	et.finish();

	//***************************************** Step3 create device buffer and map dev buf to host buf

	cl::Buffer In1_buf, In2_buf, Res_buf;
	cl::Buffer In1_buf2, In2_buf2, Res_buf2;
	et.add("Map host buffers to OpenCL buffers");
	In1_buf = cl::Buffer(context, static_cast<cl_mem_flags>( CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY),
							sizeof(dt) * LEN, in1);
	In2_buf = cl::Buffer(context, static_cast<cl_mem_flags>( CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY),
							sizeof(dt) * LEN, in2);

	Res_buf = cl::Buffer(context, static_cast<cl_mem_flags>( CL_MEM_USE_HOST_PTR | CL_MEM_WRITE_ONLY),
							sizeof(dt) * LEN, res1);

	In1_buf2 = cl::Buffer(context, static_cast<cl_mem_flags>( CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY),
							sizeof(dt) * LEN, in1);
	In2_buf2 = cl::Buffer(context, static_cast<cl_mem_flags>( CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY),
							sizeof(dt) * LEN, in2);

	Res_buf2 = cl::Buffer(context, static_cast<cl_mem_flags>( CL_MEM_USE_HOST_PTR | CL_MEM_WRITE_ONLY),
							sizeof(dt) * LEN, res2);

	et.finish();

	int j = 0;
	et.add("Set kernel arguments");
	vadd_kernel.setArg(j++, In1_buf);
	vadd_kernel.setArg(j++, In2_buf);
	vadd_kernel.setArg(j++, Res_buf);

	j = 0;
	vmul_kernel.setArg(j++, In1_buf2);
	vmul_kernel.setArg(j++, In2_buf2);
	vmul_kernel.setArg(j++, Res_buf2);

	std::vector<cl::Event> events_write(2);
	std::vector<cl::Event> events_kernel(2);
	std::vector<cl::Event> events_read(2);

	struct timeval start_time, end_time;
	gettimeofday(&start_time, 0);

	et.add("Memory object migration enqueue");
	q.enqueueMigrateMemObjects({In1_buf,In2_buf}, 0, nullptr, &events_write[0]); //TODO start transfer, 0: host -> device; 1: opposite
	q.enqueueMigrateMemObjects({In1_buf2,In2_buf2}, 0, nullptr, &events_write[1]); //TODO start transfer, 0: host -> device; 1: opposite

	clWaitForEvents(1, (const cl_event *)&events_write[0]);
	clWaitForEvents(1, (const cl_event *)&events_write[1]);

	et.add("OCL Enqueue task");
	q.enqueueTask(vadd_kernel, &events_write, &events_kernel[0]);
	q.enqueueTask(vmul_kernel, &events_write, &events_kernel[1]);

	et.add("Wait for kernel to complete");

	clWaitForEvents(1, (const cl_event *)&events_kernel[0]);
	clWaitForEvents(1, (const cl_event *)&events_kernel[1]);

	et.add("Read back computation results");
	q.enqueueMigrateMemObjects({Res_buf}, 1, &events_kernel, &events_read[0]);
	q.enqueueMigrateMemObjects({Res_buf2}, 1, &events_kernel, &events_read[1]);

	clWaitForEvents(1, (const cl_event *)&events_read[0]);
	clWaitForEvents(1, (const cl_event *)&events_read[1]);
	et.finish();
	q.finish();

//	std::cout << "Kernel st execution time is: " <<start_time.tv_sec<<"."<< start_time.tv_usec  << " s" << std::endl;

	et.print();


}
