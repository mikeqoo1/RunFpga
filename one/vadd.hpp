#ifndef _VADD_H_
#define _VADD_H_

// Includes
#include <iostream>
#include <ap_int.h>
#include <hls_stream.h>

#define DATA_SIZE 4096

// TRIPCOUNT identifier
const int size = DATA_SIZE;

extern "C"
{
    void vadd(unsigned int *input1, unsigned int *input2, unsigned int *output, int vSize);
}

#endif