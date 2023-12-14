#include "diamond.h"
#define NUM_WORDS 16
extern "C" {
 
void diamond(vecOf16Words* vecIn, vecOf16Words* vecOut, int size)
{
  hls::stream<vecOf16Words> c0, c1, c2, c3, c4, c5;
  assert(size % 16 == 0);
 
  #pragma HLS dataflow
  load(vecIn, c0, size);
  compute_A(c0, c1, c2, size);
  compute_B(c1, c3, size);
  compute_C(c2, c4, size);
  compute_D(c3, c4,c5, size);
  store(c5, vecOut, size);
}
}
 
void load(vecOf16Words *in, hls::stream<vecOf16Words >& out, int size)
{
Loop0:
  for (int i = 0; i < size; i++)
  {
    #pragma HLS performance target_ti=32
    #pragma HLS LOOP_TRIPCOUNT max=32
    out.write(in[i]);
  }
}
 
void compute_A(hls::stream<vecOf16Words >& in, hls::stream<vecOf16Words >& out1, hls::stream<vecOf16Words >& out2, int size)
{
Loop0:
  for (int i = 0; i < size; i++)
  {
    #pragma HLS performance target_ti=32
    #pragma HLS LOOP_TRIPCOUNT max=32
    vecOf16Words t = in.read();
    out1.write(t * 3);
    out2.write(t * 3);
  }
}
void compute_B(hls::stream<vecOf16Words >& in, hls::stream<vecOf16Words >& out, int size)
{
Loop0:
  for (int i = 0; i < size; i++)
  {
    #pragma HLS performance target_ti=32
    #pragma HLS LOOP_TRIPCOUNT max=32
    out.write(in.read() + 25);
  }
}
 
 
void compute_C(hls::stream<vecOf16Words >& in, hls::stream<vecOf16Words >& out, int size)
{
Loop0:
  for (data_t i = 0; i < size; i++)
  {
    #pragma HLS performance target_ti=32
    #pragma HLS LOOP_TRIPCOUNT max=32
    out.write(in.read() * 2);
  }
}
void compute_D(hls::stream<vecOf16Words >& in1, hls::stream<vecOf16Words >& in2, hls::stream<vecOf16Words >& out, int size)
{
Loop0:
  for (data_t i = 0; i < size; i++)
  { 
    #pragma HLS performance target_ti=32
    #pragma HLS LOOP_TRIPCOUNT max=32
    out.write(in1.read() + in2.read());
  }
}
 
void store(hls::stream<vecOf16Words >& in, vecOf16Words *out, int size)
{
Loop0:
  for (int i = 0; i < size; i++)
  {
    #pragma HLS performance target_ti=32
    #pragma HLS LOOP_TRIPCOUNT max=32
    out[i] = in.read();
  }
}