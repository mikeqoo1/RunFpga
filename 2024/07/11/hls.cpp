#include "hls.hpp"
#include <ap_int.h>
#include <hls_stream.h>
using namespace hls;

// 讀取客戶資料
static void read_input(order_t *customer_data, hls::stream<order_t> &inStream, int number_of_customer)
{
mem_rd:
    for (int i = 0; i < number_of_customer; i++)
    {
#pragma HLS LOOP_TRIPCOUNT min = customers_max max = customers_max
        inStream << customer_data[i];
    }
}

// 判斷
static void compare(int *initdata, hls::stream<order_t> &inStream, hls::stream<int> &outStream, int number_of_customer)
{
execute:
    int money = 0;
    for (int i = 0; i < number_of_customer; i++)
    {
#pragma HLS LOOP_TRIPCOUNT min = customers_max max = customers_max
        order_t order = inStream.read();
        // printf("資料: [%d] \n", order);
        stock_t tempstock = order(stock_len + price_len + qty_len + bs_len - 1, price_len + qty_len + bs_len);
        // printf("stock: [%d] \n", tempstock);
        price_t tempprice = order(price_len + qty_len + bs_len - 1, price_len);
        // printf("price: [%d] \n", tempprice);
        qty_t tempqty = order(qty_len + bs_len - 1, qty_len);
        // printf("qty: [%d] \n", tempqty);
        bs_t tempbs = order(bs_len - 1, 0);
        // printf("bs: [%d] \n", tempbs);
        money = tempqty * tempprice;
        // printf("money: [%d] \n", money);
        if (i == 0)
            printf("dataassign i=%d 資料: [%d] stock: [%d] price: [%d] qty: [%d] bs: [%d]\n", i, order, tempstock, tempprice, tempqty, tempbs);

        if (tempbs == 1)
        { // 買單檢查額度
            if (5000000 < money)
                outStream.write(1);
            else
                outStream.write(0);
        }
        else if (tempbs == 2)
        { // 賣單檢查庫存
            if (initdata[i] < tempqty)
                outStream.write(1);
            else
                outStream.write(0);
        }
        else
        {
            outStream.write(0);
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
    void riskcontrol(order_t *customer_data, int *customer_ans, int customers_number)
    {
#pragma HLS INTERFACE m_axi port = customer_data bundle = gmem0 depth = 32
#pragma HLS INTERFACE m_axi port = customer_ans bundle = gmem0 depth = 32
        static hls::stream<order_t> customers_in("input_stream");
        static hls::stream<int> customers_out("output_stream");
        int initdata[] = {0, 10, 30, 4, 6, 1000, 1, 3, 40, 77};
#pragma HLS STREAM variable = customers_in depth = 32
#pragma HLS STREAM variable = customers_out depth = 32

#pragma HLS dataflow
        read_input(customer_data, customers_in, customers_number);
        compare(initdata, customers_in, customers_out, customers_number);
        write_result(customer_ans, customers_out, customers_number);
    }
}