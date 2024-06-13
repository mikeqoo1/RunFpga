#include "hls.h"

/*

void riskcontrol(hls::stream<user_t> &inputdata, hls::stream<result_t> &stock_out, hls::stream<result_t> &qty_out, hls::stream<result_t> &price_out)
{
    // 本地端的數據流 stockid 和 qty 和 price (也就是 task 的輸入輸出通道) 以及 task 本身都要聲明為 hls_thread_local, 保證 TopFunction 呼叫時是活的
    hls_thread_local hls::stream<stock_t> stockid;
    hls_thread_local hls::stream<qty_t> qty;
    hls_thread_local hls::stream<price_t> price;
    hls_thread_local hls::task t0(dataassign, inputdata, stockid, qty, price); // 處理資料
    hls_thread_local hls::task t1(checkStock, stockid, stock_out);             // 檢查庫存
    hls_thread_local hls::task t2(checkQty, qty, qty_out);                     // 檢查停止融資券
    hls_thread_local hls::task t3(checkPrice, price, price_out);               // 檢查額度
}

void dataassign(hls::stream<user_t> &in, hls::stream<stock_t> &stock, hls::stream<qty_t> &qty, hls::stream<price_t> &price)
{
    // ap_uint<20> money;

    for (int i = 0; i < num_users; i++)
    {
        user_t user = in.read();
        // printf("資料: [%d] \n", user);

        stock_t tempstock = user(stock_len + qty_len + price_len - 1, qty_len + price_len);
        // printf("stock: [%d] \n", tempstock);

        price_t tempprice = user(qty_len + price_len - 1, price_len);
        // printf("price: [%d] \n", tempprice);

        qty_t tempqty = user(qty_len - 1, 0);
        // printf("qty: [%d] \n", tempqty);

        // money = tempqty * tempprice;
        // printf("money: [%d] \n", money);

        qty.write(tempqty);
        price.write(tempprice);
        stock.write(tempstock);
    }
}

void checkStock(hls::stream<stock_t> &stock, hls::stream<result_t> &check_stock_out)
{
    for (int i = 0; i < num_users; i++)
    {
        stock_t s = stock.read();

        result_t result;
        result = 0;
        // printf("stock: %d, 結果: %d \n", s, result);
        check_stock_out.write(result);
    }
}

void checkQty(hls::stream<qty_t> &qty, hls::stream<result_t> &check_qty_out)
{
    for (int i = 0; i < num_users; i++)
    {
        qty_t q = qty.read();
        result_t result;
        if (q > qty_limit)
        {
            result = 1;
        }
        else
        {
            result = 0;
        }
        // printf("qty: %d, 結果: %d \n", q, result);
        check_qty_out.write(result);
    }
}

void checkPrice(hls::stream<price_t> &price, hls::stream<result_t> &check_price_out)
{
    for (int i = 0; i < num_users; i++)
    {
        price_t p = price.read();
        result_t result;
        if (p > price_limit)
        {
            result = 1;
        }
        else
        {
            result = 0;
        }
        // printf("price: %d, 結果: %d \n", p, result);
        check_price_out.write(result);
    }
}

*/

void riskcontrol(int *account, int *stock, int *result, int vSize)
{
    static hls::stream<int> AccStream("account_stream");
    static hls::stream<int> StocknStream("stock_stream");
    static hls::stream<int> outStream("output_stream");
#pragma HLS INTERFACE m_axi port = in1 bundle = gmem0 depth = 8
#pragma HLS INTERFACE m_axi port = in2 bundle = gmem1 depth = 8
#pragma HLS INTERFACE m_axi port = out bundle = gmem0 depth = 8

#pragma HLS dataflow
    dataassign(account, AccStream, vSize);
    dataassign(stock, StocknStream, vSize);
    checkAccount(AccStream, outStream, vSize);
    checkStock(StocknStream, outStream, vSize);
    write_result(result, outStream, vSize);
}

void dataassign(int *data, hls::stream<int> &streamdata, int vSize)
{
    std::cout << "!!!Start read!!!" << std::endl;
    for (int i = 0; i < vSize; i++)
    {
#pragma HLS LOOP_TRIPCOUNT min = size max = size
        // Blocking write command to inStream
        std::cout << "krnl read=" << data[i] << std::endl;
        streamdata << data[i];
    }
    std::cout << "!!!End read!!!" << std::endl;
}

void checkALL(hls::stream<int> &acc, hls::stream<int> &stock, hls::stream<int> &result, int vSize)
{
    std::cout << "!!!Chaeck ALL!!!" << std::endl;
    for (int i = 0; i < vSize; i++)
    {
#pragma HLS LOOP_TRIPCOUNT min = size max = size
        int a = acc.read();
        int b = stock.read();
        std::cout << "checkAccount read=" << a << std::endl;
        std::cout << "checkStock read=" << b << std::endl;
        result << a;
    }
    std::cout << "!!!End Check ALL!!!" << std::endl;
}

void write_result(int *out, hls::stream<int> &result, int vSize)
{
    std::cout << "!!!Start write!!!" << std::endl;
    for (int i = 0; i < vSize; i++)
    {
#pragma HLS LOOP_TRIPCOUNT min = size max = size
        // Blocking read command to inStream
        out[i] = result.read();
        // std::cout << "krnl write=" << out[i] << std::endl;
    }
    std::cout << "!!!End write!!!" << std::endl;
}
