extern "C" {
void matrix_mul(ap_int<8> A[4][4], ap_int<8> B[4][4], ap_int<16> C[4][4]) {
    #pragma HLS ARRAY_RESHAPE dim=2 type=complete variable=A
    #pragma HLS ARRAY_RESHAPE dim=1 type=complete variable=B
    #pragma HLS INTERFACE m_axi port = A bundle = gmem0
    #pragma HLS INTERFACE m_axi port = B bundle = gmem1
    #pragma HLS INTERFACE m_axi port = C bundle = gmem0

	row: for(int i=0;i<4;i++) {
        #pragma HLS PIPELINE off
        #pragma HLS INLINE off
		col: for(int j=0;j<4;j++) {
            #pragma HLS PIPELINE II=1
            #pragma HLS INLINE off
			C[i][j]=0;
			// 循環乘四次，在相加
			product: for(int k=0;k<4;k++) {
                #pragma HLS PIPELINE off
                #pragma HLS INLINE off
				C[i][j]=C[i][j]+A[i][k]*B[k][j];
			}
		}
	}
}
}
