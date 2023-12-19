#include <algorithm>
#include <iostream>
#include "ap_fixed.h"
#define DATA_SIZE 4096

int main(int argc, char** argv) {
	if (argc != 2) {
		std::cout << "Usage: " << argv[0] << " <XCLBIN File>" << std::endl;
		return -1;
	}

	std::string binaryFile = argv[1]; //binary_container_1.xclbin
	cl_int err;
	cl::Context context;
	cl::Kernel krnl_matrix_mul;
	cl::CommandQueue q;

	ap_int<8>  A[4][4];
	ap_int<8>  B[4][4];
	ap_int<16> C[4][4];

	// 初始化值
	for(int i=0;i<4;i++)
		for(int j=0;j<4;j++) {
		A[i][j]=i*4+j;
		B[i][j]=A[i][j];
		std::cout << "A[" << i << "," << j << "]=" << A[i][j];
		std::cout << ", B[" << i << "," << j << "]=" << B[i][j] << std::endl;
	}

	// 拿 U50
	auto devices = xcl::get_xil_devices();
	// 讀執行檔
	auto fileBuf = xcl::read_binary_file(binaryFile);
	cl::Program::Binaries bins{{fileBuf.data(), fileBuf.size()}};
	bool valid_device = false;
	for (unsigned int i = 0; i < devices.size(); i++) {
		auto device = devices[i];
		// Creating Context and Command Queue for selected Device
		OCL_CHECK(err, context = cl::Context(device, nullptr, nullptr, nullptr, &err));
		OCL_CHECK(err, q = cl::CommandQueue(context, device, CL_QUEUE_PROFILING_ENABLE, &err));
		std::cout << "Trying to program device[" << i << "]: " << device.getInfo<CL_DEVICE_NAME>() << std::endl;
		cl::Program program(context, {device}, bins, nullptr, &err);
		if (err != CL_SUCCESS) {
			std::cout << "Failed to program device[" << i << "] with xclbin file!\n";
		} else {
			std::cout << "Device[" << i << "]: program successful!\n";
			OCL_CHECK(err, krnl_vector_add = cl::Kernel(program, "matrix_mul", &err));
			valid_device = true;
			break; // we break because we found a valid device
		}
	}
	if (!valid_device) {
		std::cout << "Failed to program any device found, exit!\n";
		return -1;
	}
}
