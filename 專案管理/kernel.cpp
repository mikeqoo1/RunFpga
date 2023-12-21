#include <ap_int.h>
#include <hls_stream.h>

#define DATA_SIZE 4096

// TRIPCOUNT identifier
const int c_size = DATA_SIZE;

// 從記憶體讀取資料並寫入 inStream
static void read_input(unsigned int *in, hls::stream<unsigned int> &inStream, int size)
{
// Auto-pipeline is going to apply pipeline to this loop
mem_rd:
    for (int i = 0; i < size; i++)
    {
#pragma HLS LOOP_TRIPCOUNT min = c_size max = c_size
        // Blocking write command to inStream
        inStream << in[i];
    }
}

// 從 inStream 讀取輸入資料並將結果寫入 outStream
static void compute_add(hls::stream<unsigned int> &inStream, hls::stream<unsigned int> &outStream, int inc, int size)
{
// Auto-pipeline is going to apply pipeline to this loop
execute:
    for (int i = 0; i < size; i++)
    {
#pragma HLS LOOP_TRIPCOUNT min = c_size max = c_size
        // Blocking read command from inStream and Blocking write command to outStream
        outStream << (inStream.read() + inc);
    }
}

// 把結果寫入 outStream 傳回去
static void write_result(unsigned int *out, hls::stream<unsigned int> &outStream, int size)
{
// Auto-pipeline is going to apply pipeline to this loop
mem_wr:
    for (int i = 0; i < size; i++)
    {
#pragma HLS LOOP_TRIPCOUNT min = c_size max = c_size
        // Blocking read command to inStream
        out[i] = outStream.read();
    }
}

extern "C"
{
    /*
        Vector Addition Kernel Implementation using dataflow
        Arguments:
            in   (input)  --> Input Vector
            out  (output) --> Output Vector
            inc  (input)  --> Increment
            size (input)  --> Size of Vector in Integer
       */
    void adder(unsigned int *in, unsigned int *out, int inc, int size)
    {
        // Adding names for the streams. It allows the name to be used in reporting.
        // Vivado HLS
        // automatically checks to ensure all elements from an input stream are read
        // during sw emulation.
        static hls::stream<unsigned int> inStream("input_stream");
        static hls::stream<unsigned int> outStream("output_stream");
#pragma HLS STREAM variable = inStream depth = 32
#pragma HLS STREAM variable = outStream depth = 32
        //  HLS STREAM variable=<name> depth=<size> pragma is used to define the Stream
        //  depth. For this example, Depth 32 is defined. Which means that Stream can
        //  hold
        //  maximum 32 outstanding elements at a given time. If Stream is full, any
        //  further
        //  blocking write command from producer will go into wait state until consumer
        //  reads some elements from stream. Similarly if Stream is empty (no element in
        //  Stream)
        //  any blocking read command from consumer will go into wait state until
        //  producer
        //  writes elements to Stream. This blocking read and write allow consumer and
        //  producer to synchronize each other.

#pragma HLS dataflow
        // dataflow pragma instruct compiler to run following three APIs in parallel
        read_input(in, inStream, size);
        compute_add(inStream, outStream, inc, size);
        write_result(out, outStream, size);
    }
}