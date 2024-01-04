#include "xcl2.hpp"
#include <vector>
#include <iostream>
#include <xrt/xrt_device.h>
#include <experimental/xrt_xclbin.h>
#include <xrt/xrt_bo.h>
#include <xrt/xrt_kernel.h>
#include <experimental/xrt_ip.h>
#define DATA_SIZE 4096
int main(int argc, char **argv)
{
    if (argc != 2)
    {
        std::cout <<"FPGA 執行檔案:" << argv [0] << "<XCLBIN File>" << std::endl;
        return EXIT_FAILURE;
    }

    auto binaryFile = argv[1];

    // 分配記憶體
    cl_int err;
    cl::Context context;
    cl::CommandQueue q;
    cl::Kernel risk_check;
    size_t vector_size_bytes = sizeof(int) * DATA_SIZE;

    std::vector<int, aligned_allocator<int>> customer_data(DATA_SIZE); //= {50, 66, 78, 105, 123, 5, 689, 456, 98, 24};
    std::vector<int, aligned_allocator<int>> customer_ans(DATA_SIZE);

    // 結果應該長這樣
    std::vector<int, aligned_allocator<int>> customer_ans_sw(DATA_SIZE); //= {0, 0, 0, 999, 999, 0, 999, 999, 0, 0};

    // 產生資料
    for (int i = 0; i < 10; i++)
    {
        if (i == 0)
        {
            customer_data[i] = 50;
            customer_ans_sw[i] = 0;
        }
        else if (i == 1)
        {
            customer_data[i] = 66;
            customer_ans_sw[i] = 0;
        }
        else if (i == 2)
        {
            customer_data[i] = 78;
            customer_ans_sw[i] = 0;
        }
        else if (i == 3)
        {
            customer_data[i] = 105;
            customer_ans_sw[i] = 999;
        }
        else if (i == 4)
        {
            customer_data[i] = 123;
            customer_ans_sw[i] = 999;
        }
        else if (i == 5)
        {
            customer_data[i] = 5;
            customer_ans_sw[i] = 0;
        }
        else if (i == 6)
        {
            customer_data[i] = 689;
            customer_ans_sw[i] = 999;
        }
        else if (i == 7)
        {
            customer_data[i] = 456;
            customer_ans_sw[i] = 999;
        }
        else if (i == 8)
        {
            customer_data[i] = 98;
            customer_ans_sw[i] = 0;
        }
        else if (i == 9)
        {
            customer_data[i] = 24;
            customer_ans_sw[i] = 0;
        }
        customer_ans[i] = 0;
    }

    // 去拿板子資訊
    auto devices = xcl::get_xil_devices();

    // 板子內核的讀取
    auto fileBuf = xcl::read_binary_file(binaryFile);
    cl::Program::Binaries bins{{fileBuf.data(), fileBuf.size()}};
    bool valid_device = false;
    for (unsigned int i = 0; i < devices.size(); i++)
    {
        auto device = devices[i];
        // Creating Context and Command Queue for selected Device
        OCL_CHECK(err, context = cl::Context(device, nullptr, nullptr, nullptr, &err));
        OCL_CHECK(err, q = cl::CommandQueue(context, device, CL_QUEUE_PROFILING_ENABLE, &err));

        std::cout << "Trying to program device[" << i << "]: " << device.getInfo<CL_DEVICE_NAME>() << std::endl;
        cl::Program program(context, {device}, bins, nullptr, &err);
        if (err != CL_SUCCESS)
        {
            std::cout << "Failed to program device[" << i << "] with xclbin file!\n";
        }
        else
        {
            std::cout << "Device[" << i << "]: program successful!\n";
            OCL_CHECK(err, risk_check = cl::Kernel(program, "risk_check", &err));
            valid_device = true;
            break; // we break because we found a valid device
        }
    }
    if (!valid_device)
    {
        std::cout << "Failed to program any device found, exit!\n";
        exit(EXIT_FAILURE);
    }

    // 把本地資料丟給板子
    OCL_CHECK(err, cl::Buffer buffer_input(context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, vector_size_bytes,
                                           customer_data.data(), &err));
    OCL_CHECK(err, cl::Buffer buffer_output(context, CL_MEM_USE_HOST_PTR | CL_MEM_WRITE_ONLY, vector_size_bytes,
                                            customer_ans.data(), &err));

    // 設定核心的參數
    int narg = 0;
    OCL_CHECK(err, err = risk_check.setArg(narg++, buffer_input));
    OCL_CHECK(err, err = risk_check.setArg(narg++, buffer_output));
    OCL_CHECK(err, err = risk_check.setArg(narg++, 10));

    // 把 input 資料丟到板子的記憶體
    OCL_CHECK(err, err = q.enqueueMigrateMemObjects({buffer_input}, 0 /* 0 means from host*/));

    // 啟動核心
    OCL_CHECK(err, err = q.enqueueTask(risk_check));

    // 把板子上的結果複製到本地端
    OCL_CHECK(err, err = q.enqueueMigrateMemObjects({buffer_output}, CL_MIGRATE_MEM_OBJECT_HOST));
    q.finish();

    // OPENCL HOST CODE AREA END

    int match = 0;
    // 顯示結果
    for (int i = 0; i < 10; i++)
    {
        std::cout <<"硬體結果 =" << customer_ans [i] << "|";
        std::cout <<"軟體結果 =" << customer_ans_sw [i] << std::endl;
        if (customer_ans[i] != customer_ans_sw[i])
        {
            std::cout << "Error: Result mismatch" << std::endl;
            std::cout << "i = " << i << " CPU result = " << customer_ans_sw[i]
                      << " Device result = " << customer_ans[i] << std::endl;
            match = 1;
            break;
        }
    }

    // std::cout << "TEST " << (match ? "FAILED" : "PASSED") << std::endl;
    return match;
}
