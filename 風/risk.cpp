
/*
把要檢查的資料從本地端傳給板子上
板子上做簡單的判斷
例如傳遞客戶的資料回應對應的結果
然後可以傳遞多種 CASE
*/

#include <ap_int.h>
#include <hls_stream.h>
using namespace hls;

// 預設最大處理數量
const int customers_max = 50;

// LOOP_TRIPCOUNT 會要求指定循環次數的最小、最大和平均值

// 讀取客戶資料
static void read_input(int *customer_data, hls::stream<int> &inStream, int number_of_customer)
{
mem_rd:
    for (int i = 0; i < number_of_customer; i++)
    {
#pragma HLS LOOP_TRIPCOUNT min = customers_max max = customers_max
        inStream << customer_data[i];
    }
}

// 判斷資料
static void compare(hls::stream<int> &inStream, hls::stream<int> &outStream, int number_of_customer)
{
execute:
    int all = 0;
    int use = 0;
    int count = 0;
    for (int i = 0; i < number_of_customer; i++)
    {
#pragma HLS LOOP_TRIPCOUNT min = customers_max max = customers_max
        if (count % 2 == 0)
            all = inStream.read();
        else
            use = inStream.read();

        count = count + 1;
        if (use > 0)
        {
            if (use > all)
            {
                outStream << 999;
            }
            else
            {
                outStream << 0;
            }
        } else {
            outStream << 111;
        }
    }
}

// 回傳對應的結果
static void write_result(int *customer_ans, hls::stream<int> &outStream, int number_of_customer)
{
mem_wr:
    for (int i = 0; i < number_of_customer; i++)
    {
#pragma HLS LOOP_TRIPCOUNT min = customers_max max = customers_max
        customer_ans[i] = outStream.read();
    }
}

extern "C"
{
    void risk_check(int *customer_data, int *customer_ans, int customers_number)
    {
        static hls::stream<int> customers_in("input_stream");
        static hls::stream<int> customers_out("output_stream");
#pragma HLS STREAM variable = customers_in depth = 32
#pragma HLS STREAM variable = customers_out depth = 32

#pragma HLS dataflow
        read_input(customer_data, customers_in, customers_number);
        compare(customers_in, customers_out, customers_number);
        write_result(customer_ans, customers_out, customers_number);
    }
}