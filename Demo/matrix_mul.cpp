#include "matrix_mul.h"

void matrix_mul(ap_int<8> A[4][4], ap_int<8> B[4][4], ap_int<16> C[4][4]) {
	row: for(int i=0;i<4;i++) {
		col: for(int j=0;j<4;j++) {
			C[i][j]=0;
			// 循環乘四次，在相加
			product: for(int k=0;k<4;k++) {
				C[i][j]=C[i][j]+A[i][k]*B[k][j];
			}
		}
	}
}