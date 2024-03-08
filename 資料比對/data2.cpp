#include "data2.h"

void riskcontrol(hls::stream<user_t> &inputdata)
{
    // 本地端的數據流 stockid 和 qty 和 price (也就是 task 的輸入輸出通道) 以及 task 本身都要聲明為 hls_thread_local, 保證 TopFunction 呼叫時是活的
    hls_thread_local hls::stream<stock_t> stockid;
    hls_thread_local hls::stream<qty_t> qty;
    hls_thread_local hls::stream<price_t> price;
    for (int i = 0; i < num_users; i++)
    {
        user_t user = inputdata.read();

        qty_t tempqty = user(qty_len - 1, 0);
        printf("qty: [%d] \n", tempqty);
        user = user >> qty_len;

        price_t tempprice = user(price_len - 1, 0);
        printf("price: [%d] \n", tempprice);
        user = user >> price_len;

        stock_t tempstock = user(stock_len - 1, 0);
        printf("stock: [%d] \n", tempstock);
        user = user >> stock_len;

        qty.write(tempqty);
        price.write(tempprice);
        stockid.write(tempstock);
    }
}