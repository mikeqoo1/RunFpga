#include "hls.hpp"
#include <ap_int.h>
#include <hls_stream.h>
using namespace hls;

// 讀取客戶資料
static void read_input(order_t *customer_data, hls::stream<order_t> &inStream, int order_record)
{
mem_rd:
    for (int i = 0; i < order_record; i++)
    {
#pragma HLS LOOP_TRIPCOUNT min = order_record_max max = order_record_max
        inStream << customer_data[i];
    }
}

// 判斷
static void compare(int *initdata, hls::stream<order_t> &inStream, hls::stream<int> &outStream, int order_record)
{
execute:
    int money = 0;
    for (int i = 0; i < order_record; i++)
    {
#pragma HLS LOOP_TRIPCOUNT min = order_record_max max = order_record_max
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
        money = tempqty * tempprice * 1000;
        // printf("money: [%d] \n", money);
        if (i == 0 || i == 1 || i == 2 || i == 3 || i == 4 || i == 5 || i == 6 || i == 7 || i == 8 || i == 9)
            printf("dataassign i=%d 資料: [%d] stock: [%d] price: [%d] qty: [%d] bs: [%d]\n", i, order, tempstock, tempprice, tempqty, tempbs);

        if (tempbs == 1) // 買單檢查額度
        {
            use_amt += money;
            printf("單筆價金 i=%d 已用投資額度 i=%d \n", money, use_amt);
            if (5000000 < use_amt)
                outStream.write(1);
            else
                outStream.write(0);
        }
        else if (tempbs == 2) // 賣單檢查庫存
        {
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
    /* 非同步版本 失敗 */
    /*
execute:
    int money = 0;
    int i = 0;
    int check = 0;
    int index;
    while (check != 0)
    {
        if (!inStream.empty())
        {
            order_t order;
            if (inStream.read_nb(order))
            {
                //order_t order = inStream.read();
                // printf("資料: [%d] \n", order);
                stock_t tempstock = order(stock_len + price_len + qty_len + bs_len - 1, price_len + qty_len + bs_len);
                // printf("stock: [%d] \n", tempstock);
                price_t tempprice = order(price_len + qty_len + bs_len - 1, price_len);
                // printf("price: [%d] \n", tempprice);
                qty_t tempqty = order(qty_len + bs_len - 1, qty_len);
                // printf("qty: [%d] \n", tempqty);
                bs_t tempbs = order(bs_len - 1, 0);
                // printf("bs: [%d] \n", tempbs);
                money = tempqty * tempprice * 1000;
                // printf("money: [%d] \n", money);
                if (i == 0 || i == 1 || i == 2 || i == 3 || i == 4 || i == 5 || i == 6 || i == 7 || i == 8 || i == 9)
                    printf("dataassign i=%d 資料: [%d] stock: [%d] price: [%d] qty: [%d] bs: [%d]\n", i, order, tempstock, tempprice, tempqty, tempbs);

                if (tempbs == 1) // 買單檢查額度
                {
                    use_amt += money;
                    printf("單筆價金 i=%d 已用投資額度 i=%d \n", money, use_amt);
                    if (5000000 < use_amt)
                        outStream.write(1);
                    else
                        outStream.write(0);
                }
                else if (tempbs == 2) // 賣單檢查庫存
                {
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
        check = 1;
    }
    */
}

// 回傳對應的結果
static void write_result(int *customer_ans, hls::stream<int> &outStream, int order_record)
{
mem_wr:
    for (int i = 0; i < order_record; i++)
    {
#pragma HLS LOOP_TRIPCOUNT min = order_record_max max = order_record_max
        customer_ans[i] = outStream.read();
    }
    /* 非同步版本 失敗 */
    /*
mem_wr:
    int i = 0;
    int check = 0;
    while (check != 0)
    {
        if (!outStream.empty())
        {
            int ans;
            if (outStream.read_nb(ans))
            {
                customer_ans[i] = ans;
            }
        }
        check = 1;
    }
*/
}

extern "C"
{
    void riskcontrol(order_t *customer_data, int *customer_ans, int order_record)
    {
#pragma HLS INTERFACE m_axi port = customer_data bundle = gmem0 depth = 64
#pragma HLS INTERFACE m_axi port = customer_ans bundle = gmem0 depth = 64
        static hls::stream<order_t> customers_in("input_stream");
        static hls::stream<int> customers_out("output_stream");
        int initdata[] = {0, 10, 30, 4, 6, 1000, 10, 3, 40, 77};
#pragma HLS STREAM variable = customers_in depth = 32
#pragma HLS STREAM variable = customers_out depth = 32

#pragma HLS dataflow
        read_input(customer_data, customers_in, order_record);
        compare(initdata, customers_in, customers_out, order_record);
        write_result(customer_ans, customers_out, order_record);
    }
}