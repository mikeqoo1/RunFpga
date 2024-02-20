#include "hls_task.h"
#include <iostream>
#define N 10

/*
F15 庫存不足
F16 停止融券 or 沒有配券
F22 額度不夠
*/

void riskcontrol(hls::stream<int> &inputdata,
             hls::stream<int> &F15, hls::stream<int> &F16, hls::stream<int> &F22);