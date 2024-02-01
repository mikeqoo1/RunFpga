#include "seiko.h"

extern "C"
{
    void checkA(int *a, int *b, int *c, int *out_r, const int n_elements)
    {
#pragma HLS INTERFACE m_axi port = a offset = slave bundle = gmem
#pragma HLS INTERFACE s_axilite port = a
#pragma HLS INTERFACE m_axi port = b offset = slave bundle = gmem
#pragma HLS INTERFACE s_axilite port = b
#pragma HLS INTERFACE m_axi port = c offset = slave bundle = gmem
#pragma HLS INTERFACE s_axilite port = c
#pragma HLS INTERFACE m_axi port = out_r offset = slave bundle = gmem
#pragma HLS INTERFACE s_axilite port = out_r
#pragma HLS INTERFACE s_axilite port = n_elements
#pragma HLS INTERFACE s_axilite port = return

        int arrayA[BUFFER_SIZE];

    check:
        for (int i = 0; i < n_elements; i += BUFFER_SIZE)
        {
#pragma HLS LOOP_TRIPCOUNT min = c_len max = c_len
            int size = BUFFER_SIZE;
            // boundary check
            if (i + size > n_elements)
                size = n_elements - i;

        // Burst reading A
        // Auto-pipeline is going to apply pipeline to these loops
        readAandwriteOut:
            for (int j = 0; j < size; j++)
            {
#pragma HLS LOOP_TRIPCOUNT min = c_size max = c_size
                arrayA[j] = a[i + j];
                if (arrayA[j] < 10)
                    out_r[i + j] = 1111;
                else
                    out_r[i + j] = 0;
            }
        }
    }
}