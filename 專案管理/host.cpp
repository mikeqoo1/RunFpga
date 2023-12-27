#include "xcl2.hpp"
#include <vector>
#include <iostream>
#include <xrt/xrt_device.h>
#include <experimental/xrt_xclbin.h>
#include <xrt/xrt_bo.h>
#include <xrt/xrt_kernel.h>
#include <experimental/xrt_ip.h>
#include <experimental/aie.h>
#include <experimental/graph.h>
#define DATA_SIZE 4096
#define INCR_VALUE 10

int main(int argc, char** argv) {
	if (argc != 2) {
		std::cout <<"FPGA 執行檔案:" << argv [0] << "<XCLBIN File>" << std::endl;
		return EXIT_FAILURE;
	}

	auto binaryFile = argv[1];

	// 分配記憶體
	cl_int err;
	cl::Context context;
	cl::CommandQueue q;
	cl::Kernel krnl_adder;
	size_t vector_size_bytes = sizeof(int) * DATA_SIZE;

	//aligned_allocator 這是 C++ 的 STL
	std::vector<int, aligned_allocator<int> > source_input(DATA_SIZE);
	std::vector<int, aligned_allocator<int> > source_hw_results(DATA_SIZE);
	std::vector<int, aligned_allocator<int> > source_sw_results(DATA_SIZE);

	// 建立測試數據和軟體結果
	for (int i = 0; i < DATA_SIZE; i++) {
		source_input[i] = i;
		source_sw_results[i] = i + INCR_VALUE;
		source_hw_results[i] = 0;
		std::cout << "source_input=" << source_input[i] << "|source_sw_results=" << source_sw_results[i] << "|source_hw_results=" << source_hw_results[i] << std::endl;
	}

	// 去拿板子資訊
	auto devices = xcl::get_xil_devices();

	// 板子內核的讀取
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
			OCL_CHECK(err, krnl_adder = cl::Kernel(program, "adder", &err));
			valid_device = true;
			break; // we break because we found a valid device
		}
	}
	if (!valid_device) {
		std::cout << "Failed to program any device found, exit!\n";
		exit(EXIT_FAILURE);
	}

	// Allocate Buffer in Global Memory
	OCL_CHECK(err, cl::Buffer buffer_input(context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, vector_size_bytes,
										   source_input.data(), &err));
	OCL_CHECK(err, cl::Buffer buffer_output(context, CL_MEM_USE_HOST_PTR | CL_MEM_WRITE_ONLY, vector_size_bytes,
											source_hw_results.data(), &err));

	int inc = INCR_VALUE;
	int size = DATA_SIZE;
	// 設定核心的參數
	int narg = 0;
	OCL_CHECK(err, err = krnl_adder.setArg(narg++, buffer_input));
	OCL_CHECK(err, err = krnl_adder.setArg(narg++, buffer_output));
	OCL_CHECK(err, err = krnl_adder.setArg(narg++, inc));
	OCL_CHECK(err, err = krnl_adder.setArg(narg++, size));

	// 把 input 資料丟到板子的記憶體
	OCL_CHECK(err, err = q.enqueueMigrateMemObjects({buffer_input}, 0 /* 0 means from host*/));

	// 啟動核心
	OCL_CHECK(err, err = q.enqueueTask(krnl_adder));

	// 把板子上的結果複製到本地端
	OCL_CHECK(err, err = q.enqueueMigrateMemObjects({buffer_output}, CL_MIGRATE_MEM_OBJECT_HOST));
	q.finish();

	// OPENCL HOST CODE AREA END

	// 比對結果
	int match = 0;
	for (int i = 0; i < DATA_SIZE; i++) {
		if (source_hw_results[i] != source_sw_results[i]) {
			std::cout << "Error: Result mismatch" << std::endl;
			std::cout << "i = " << i << " CPU result = " << source_sw_results[i]
					  << " Device result = " << source_hw_results[i] << std::endl;
			match = 1;
			break;
		}
	}

	std::cout << "TEST " << (match ? "FAILED" : "PASSED") << std::endl;
	return (match ? EXIT_FAILURE : EXIT_SUCCESS);
}


