#include "vadd.hpp"

int main()
{

    unsigned int in1[size], in2[size];
    unsigned int out[size], res[size];
    for (int i = 0; i < size; ++i)
    {
        in1[i] = i;
        in2[i] = i;
        out[i] = 0;
        res[i] = in1[i] + in2[i];
    }

    vadd(in1, in2, out, size);

    for (int i = 0; i < size; ++i)
    {
        if (res[i] != out[i])
            return EXIT_FAILURE;
    }

    std::cout << "Test passed.\n";
    return EXIT_SUCCESS;
}