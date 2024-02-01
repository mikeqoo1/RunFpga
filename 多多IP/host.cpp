#include "seiko.h"
#include "xcl2.hpp"
#include <algorithm>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <vector>
bool run_kernel(std::string &binaryFile, int krnl_id)
{
    cl_int err;
    cl::Context context;
    cl::CommandQueue q;
    cl::Kernel krnl;
    std::srand(time(NULL));
    const char *krnl_names[] = {"checkA", "checkB", "checkC"};

    int pid = getpid();
    printf("\n[PID: %d] Start Vector Operation (PARENT PPID: %d)\n", pid, getppid());

    // 定義寬度
    size_t vector_size_bytes = sizeof(int) * LENGTH;
    std::vector<int, aligned_allocator<int>> source_a (LENGTH); // 庫存不足 10 張
    std::vector<int, aligned_allocator<int>> source_b (LENGTH); // 停止融券
    std::vector<int, aligned_allocator<int>> source_c (LENGTH); // 額度不足 400 萬
    std::vector<int, aligned_allocator<int>> result_sw(LENGTH);
    std::vector<int, aligned_allocator<int>> result_hw(LENGTH);

    //generate 遍尋起始位置 結束位置 要做啥事
    std::generate(source_a.begin(), source_a.end(), []()
                  { return std::rand() % 10 + 1; }); // 1~10
    std::generate(source_b.begin(), source_b.end(), []()
                  { return std::rand() % 5; }); // 0~4
    std::generate(source_c.begin(), source_c.end(), []()
                  { return std::rand() % (499 - 50 + 1) + 50; }); // 50~499
    for (int i = 0; i < LENGTH; i++)
    {
        std::cout <<"初始值" << source_a [i] << std::endl;
        std::cout <<"初始值" << source_b [i] << std::endl;
        std::cout <<"初始值" << source_c [i] << std::endl;
        result_hw[i] = 0;
        switch (krnl_id)
        {
        case 0:
            if (source_a[i] < 10)
                result_sw[i] = 1111;
            else
                result_sw[i] = 0;
            break;
        case 1:
            if (source_b[i] != 0)
                result_sw[i] = 1031;
            else
                result_sw[i] = 0;
            break;
        case 2:
            if (source_c[i] < 400)
                result_sw[i] = 1112;
            else
                result_sw[i] = 0;
            break;
        default:
            std::cout << "Kernel ID is unknown!!" << std::endl;
        }
    }

    // OPENCL HOST CODE AREA START
    auto devices = xcl::get_xil_devices();
    printf("\n[PID: %d] Read XCLBIN file\n", pid);

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
        std::cout << "[PID: " << pid << "] Create a Program and a [ " << krnl_names[krnl_id] << " ] Kernel\n";
        cl::Program program(context, {device}, bins, nullptr, &err);
        if (err != CL_SUCCESS)
        {
            std::cout << "Failed to program device[" << i << "] with xclbin file!\n";
        }
        else
        {
            std::cout << "Device[" << i << "]: program successful!\n";
            OCL_CHECK(err, krnl = cl::Kernel(program, krnl_names[krnl_id], &err));
            valid_device = true;
            break; // we break because we found a valid device
        }
    }
    if (!valid_device)
    {
        std::cout << "Failed to program any device found, exit!\n";
        exit(EXIT_FAILURE);
    }

    OCL_CHECK(err, cl::Buffer buffer_a(context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, vector_size_bytes,
                                       source_a.data(), &err));
    OCL_CHECK(err, cl::Buffer buffer_b(context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, vector_size_bytes,
                                       source_b.data(), &err));
    OCL_CHECK(err, cl::Buffer buffer_c(context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, vector_size_bytes,
                                       source_c.data(), &err));
    OCL_CHECK(err, cl::Buffer buffer_d(context, CL_MEM_USE_HOST_PTR | CL_MEM_WRITE_ONLY, vector_size_bytes,
                                       result_hw.data(), &err));

    /* Set the kernel arguments */
    int vector_length = LENGTH;

    OCL_CHECK(err, err = krnl.setArg(0, buffer_a));
    OCL_CHECK(err, err = krnl.setArg(1, buffer_b));
    OCL_CHECK(err, err = krnl.setArg(2, buffer_c));
    OCL_CHECK(err, err = krnl.setArg(3, buffer_d));
    OCL_CHECK(err, err = krnl.setArg(4, vector_length));

    /* Copy input vectors to memory */
    printf("\n[PID: %d] Transfer the Input Data to Device\n", pid);
    OCL_CHECK(err, err = q.enqueueMigrateMemObjects({buffer_a, buffer_b}, 0 /* 0 means from host*/));

    /* Launch the kernel */
    printf("[PID: %d] Launch Kernel\n", pid);
    OCL_CHECK(err, err = q.enqueueTask(krnl));

    /* Copy result to local buffer */
    printf("[PID: %d] Transfer the Output Data from Device\n", pid);
    OCL_CHECK(err, err = q.enqueueMigrateMemObjects({buffer_c}, CL_MIGRATE_MEM_OBJECT_HOST));
    q.finish();
    // OPENCL HOST CODE AREA END

    /* Compare the results of the kernel to the simulation */
    bool krnl_match = true;

    printf("\n[PID: %d] Checking the Output Data with Golden Results...\n", pid);

    for (int i = 0; i < LENGTH; i++)
    {
        if (result_sw[i] != result_hw[i])
        {
            printf("Error: i = %d CPU result = %d FPGA Result = %d\n", i, result_sw[i], result_hw[i]);
            krnl_match = false;
            break;
        }
    }

    return krnl_match;
}

int main(int argc, char *argv[])
{
    int iter = 3;

    if (argc != 2)
    {
        std::cout << "Usage: " << argv[0] << " <XCLBIN File> " << std::endl;
        return EXIT_FAILURE;
    }

    std::string binaryFile = argv[1];

    // Setting XCL_MULTIPROCESS_MODE
    std::cout << "Set the env variable for Multi Process Support (MPS)" << std::endl;
    char mps_env[] = "XCL_MULTIPROCESS_MODE=1";
    if (putenv(mps_env) != 0)
    {
        std::cout << "putenv failed" << std::endl;
    }
    else
        std::cout << "Env variable: XCL_MULTIPROCESS_MODE: " << getenv("XCL_MULTIPROCESS_MODE") << std::endl;

    bool result = true;

    std::cout << "Now create (" << iter << ") CHILD processes" << std::endl;
    for (int i = 0; i < iter; i++)
    {
        if (fork() == 0)
        {
            printf("[CHILD] PID %d from [PARENT] PPID %d\n", getpid(), getppid());
            result = run_kernel(binaryFile, i);
            exit(!(result));
        }
    }

    // Need to wait for all child process to complete
    std::cout << "\n[PID: " << getpid() << "] PARENT WAITS CHILD TO FINISH.\n\n"
              << std::endl;
    for (int i = 0; i < iter; i++)
    {
        int status = 0;
        int child = wait(&status);
        std::cout << "[PID: " << getpid() << "] child: " << child << " exited with WIFEXITED: " << WIFEXITED(status)
                  << " and WEXITSTATUS: " << WEXITSTATUS(status) << std::endl;
        if (WEXITSTATUS(status) and WIFEXITED(status))
            result = false;
    }

    std::cout << "TEST " << ((result) ? "PASSED" : "FAILED") << std::endl;
    return ((result) ? EXIT_SUCCESS : EXIT_FAILURE);
}