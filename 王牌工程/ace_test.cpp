#include <iostream>
#include "ace_kernel.h"

int main()
{
    hls::stream<int, 9> inputdata;
    hls::stream<int, 3> f15;
    hls::stream<int, 3> f16;
    hls::stream<int, 3> f22;
    int data[9] = {2330, 8000, 666, 6016, 470000, 333, 1102, 10000, 99};
    for (int i = 0; i < 9; i++)
        inputdata.write(data[i]);

    riskcontrol(inputdata, f15, f16, f22);

    for (int i = 0; i < 3; i++)
    {
        std::cout << "f15 = " << f15.read() << std::endl;
        std::cout << "f16 = " << f16.read() << std::endl;
        std::cout << "f22 = " << f22.read() << std::endl;
    }

/*
    int data[9] = {2330, 8000, 666, 6016, 470000, 333, 1102, 10000, 99};
    int f15[3] = {0, 0, 0};
    int f16[3] = {0, 0, 0};
    int f22[3] = {0, 0, 0};

    riskcontrol(data, f15, f16, f22);

    for (int i = 0; i < 3; i++)
    {
        std::cout << "i=" << i << std::endl;
        std::cout << "f15 = " << f15[i] << std::endl;
        std::cout << "f16 = " << f16[i] << std::endl;
        std::cout << "f22 = " << f22[i] << std::endl;
    }
*/
    return 0;
}
