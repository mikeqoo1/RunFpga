#ifndef __MATRIX_MUL__
#define __MATRIX_MUL__

// HLS 的任意寬度標頭檔
#include "ap_fixed.h"

// A 跟 B 相乘後得到 C
void matrix_mul(ap_int<8> A[4][4], ap_int<8> B[4][4], ap_int<16> C[4][4]);

#endif