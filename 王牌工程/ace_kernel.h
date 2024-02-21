#include "hls_task.h"
#include <iostream>
#define N 10

/*
F15 庫存不足
F16 停止融券 or 沒有配券
F22 額度不夠
*/

// Top function
void riskcontrol(hls::stream<int> &inputdata,
                 hls::stream<int> &F15, hls::stream<int> &F16, hls::stream<int> &F22);
// Sub function
void dataassign(hls::stream<int> &in,
                hls::stream<int> &stock, hls::stream<int> &qty, hls::stream<int> &price);
void checkF15(hls::stream<int> &qty, hls::stream<int> &f15out);
void checkF16(hls::stream<int> &stock, hls::stream<int> &f16out);
void checkF22(hls::stream<int> &price, hls::stream<int> &f22out);

/*
// DATAFLOW 版本
void riskcontrol(int In[N], int F15Out[N], int F16Out[N], int F22Out[N]);

// Sub functions
void dataassign(int In[N], int stock[N], int qty[N], int price[N]);
void checkF15(int qty[N], int f15out[N]);
void checkF16(int stock[N], int f15out[N]);
void checkF22(int price[N], int f22out[N]);
*/