#include <ap_int.h>
#include <hls_stream.h>
#include <iostream>
#include <stdio.h>
using namespace hls;

#define stock_len 14
#define qty_len 14
#define price_len 20
#define bs_len 2
#define order_len stock_len + qty_len + price_len + bs_len

typedef ap_uint<order_len> order_t;
typedef ap_uint<stock_len> stock_t;
typedef ap_uint<qty_len> qty_t;
typedef ap_uint<price_len> price_t;
typedef ap_uint<bs_len> bs_t;

static int use_amt = 0;

// 預設最大處理數量
const int order_record_max = 50;

// 讀取客戶資料
static void read_input(order_t *customer_data, hls::stream<order_t> &inStream, int order_record);

// 判斷資料
static void compare(int *initdata, hls::stream<order_t> &inStream, hls::stream<int> &outStream, int order_record);

// 回傳對應的結果
static void write_result(int *customer_ans, hls::stream<int> &outStream, int order_record);

extern "C"
{
    void riskcontrol(order_t *customer_data, int *customer_ans, int order_record);
}