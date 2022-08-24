#include "event_timer.hpp"
#include "xcl2.hpp"
#include <sys/time.h>

typedef int dt;
#define VMUL_XCL "../../test_system_hw_link/Hardware/binary_container_2.xclbin"


void vmul_op(dt *in1, dt *in2, dt *res, int LEN){
	EventTimer et;

	cl_int fail;

	//***************************************** Step1 platform related operations
	std::vector<cl::Device> devices = xcl::get_xil_devices();
	cl::Device device = devices[0]; //the device 0

	//***************************************** Step2 Creating Context and Command Queue for selected Device


	et.add("OpenCL Initialization");

	cl::Context context(device, NULL, NULL, NULL, &fail); //initial OpenCL environment

	cl::CommandQueue q(context, device, CL_QUEUE_PROFILING_ENABLE | CL_QUEUE_OUT_OF_ORDER_EXEC_MODE_ENABLE, &fail); //command queue

	cl::Program::Binaries xclBins = xcl::import_binary_file(VMUL_XCL); //load the binary file
	devices.resize(1);
	cl::Program program(context, devices, xclBins, NULL, &fail); // pragram the fpga

	cl::Kernel vmul_kernel;
	vmul_kernel = cl::Kernel(program, "vmul_kernel", &fail);//The name "vmul_kernel" should be same as your kernel name .xo
	et.finish();

	//***************************************** Step3 create device buffer and map dev buf to host buf

	cl::Buffer In1_buf, In2_buf, Res_buf;
	et.add("Map host buffers to OpenCL buffers");
	In1_buf = cl::Buffer(context, static_cast<cl_mem_flags>( CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY),
							sizeof(dt) * LEN, in1);
	In2_buf = cl::Buffer(context, static_cast<cl_mem_flags>( CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY),
							sizeof(dt) * LEN, in2);

	Res_buf = cl::Buffer(context, static_cast<cl_mem_flags>( CL_MEM_USE_HOST_PTR | CL_MEM_WRITE_ONLY),
							sizeof(dt) * LEN, res);

	et.finish();

	int j = 0;
	et.add("Set kernel arguments");
	vmul_kernel.setArg(j++, In1_buf);
	vmul_kernel.setArg(j++, In2_buf);
	vmul_kernel.setArg(j++, Res_buf);

	std::vector<cl::Event> events_write(1);
	std::vector<cl::Event> events_kernel(1);
	std::vector<cl::Event> events_read(1);

	struct timeval start_time, end_time;
	gettimeofday(&start_time, 0);

	et.add("Memory object migration enqueue");
	q.enqueueMigrateMemObjects({In1_buf,In2_buf}, 0, nullptr, &events_write[0]); //TODO start transfer, 0: host -> device; 1: opposite

	clWaitForEvents(1, (const cl_event *)&events_write[0]);

	et.add("OCL Enqueue task");
	q.enqueueTask(vmul_kernel, &events_write, &events_kernel[0]);

	et.add("Wait for kernel to complete");

	clWaitForEvents(1, (const cl_event *)&events_kernel[0]);

	et.add("Read back computation results");
	q.enqueueMigrateMemObjects({Res_buf}, 1, &events_kernel, &events_read[0]);
	clWaitForEvents(1, (const cl_event *)&events_read[0]);
	et.finish();
	q.finish();

//	std::cout << "Kernel st execution time is: " <<start_time.tv_sec<<"."<< start_time.tv_usec  << " s" << std::endl;

	et.print();
}
