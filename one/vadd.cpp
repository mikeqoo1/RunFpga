#include "vadd.hpp"
#include <stdint.h>
#include <hls_stream.h>

static void read_input(unsigned int *in, hls::stream<unsigned int> &inStream, int vSize)
{
// Auto-pipeline is going to apply pipeline to this loop
mem_rd:
    for (int i = 0; i < vSize; i++)
    {
#pragma HLS LOOP_TRIPCOUNT min = size max = size
        inStream << in[i];
    }
}

static void compute_add(hls::stream<unsigned int> &inStream1,
                        hls::stream<unsigned int> &inStream2,
                        hls::stream<unsigned int> &outStream,
                        int vSize)
{
// Auto-pipeline is going to apply pipeline to this loop
execute:
    for (int i = 0; i < vSize; i++)
    {
#pragma HLS LOOP_TRIPCOUNT min = size max = size
        outStream << (inStream1.read() + inStream2.read());
    }
}

static void write_result(unsigned int *out, hls::stream<unsigned int> &outStream, int vSize)
{
// Auto-pipeline is going to apply pipeline to this loop
mem_wr:
    for (int i = 0; i < vSize; i++)
    {
#pragma HLS LOOP_TRIPCOUNT min = size max = size
        out[i] = outStream.read();
    }
}

extern "C"
{
    /*
        Vector Addition Kernel Implementation using dataflow
        Arguments:
            in1   (input)  --> Input Vector 1
            in2   (input)  --> Input Vector 2
            out  (output) --> Output Vector
            size (input)  --> Size of Vector in Integer
       */
    void vadd(unsigned int *in1, unsigned int *in2, unsigned int *out, int vSize)
    {
        static hls::stream<unsigned int> inStream1("input_stream_1");
        static hls::stream<unsigned int> inStream2("input_stream_2");
        static hls::stream<unsigned int> outStream("output_stream");

#pragma HLS INTERFACE m_axi port = in1 bundle = gmem0
#pragma HLS INTERFACE m_axi port = in2 bundle = gmem1
#pragma HLS INTERFACE m_axi port = out bundle = gmem0

#pragma HLS dataflow
        // dataflow pragma instruct compiler to run following three APIs in parallel
        read_input(in1, inStream1, vSize);
        read_input(in2, inStream2, vSize);
        compute_add(inStream1, inStream2, outStream, vSize);
        write_result(out, outStream, vSize);
    }
}
