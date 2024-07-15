#include "hls.hpp"

void riskcontrol(hls::stream<user_t> &inputdata, int *account, int *stock, hls::stream<result_t> &stock_out, hls::stream<result_t> &qty_out, hls::stream<result_t> &price_out)
{

    static hls::stream<int> DB_stock("stock_stream");
    dataassign2(stock, DB_stock);
    static hls::stream<int> DB_account("account_stream");
    dataassign2(account, DB_account);

    // 本地端的數據流 stockid 和 qty 和 price (也就是 task 的輸入輸出通道) 以及 task 本身都要聲明為 hls_thread_local, 保證 TopFunction 呼叫時是活的
    hls::stream<stock_t> stockid;
    hls::stream<qty_t> qty;
    hls::stream<price_t> price;
    dataassign(inputdata, stockid, qty, price);    // 處理資料
    checkStock(stockid, DB_stock, stock_out);      // 檢查庫存
    checkQty(qty, qty_out);                        // 檢查停止融資券
    checkPrice(price, qty, DB_account, price_out); // 檢查額度
}

void dataassign(hls::stream<user_t> &in, hls::stream<stock_t> &stock, hls::stream<qty_t> &qty, hls::stream<price_t> &price)
{
    // ap_uint<20> money;

    for (int i = 0; i < num_users; i++)
    {
        user_t user = in.read();
        printf("資料: [%d] \n", user);

        stock_t tempstock = user(stock_len + qty_len + price_len - 1, qty_len + price_len);
        printf("stock: [%d] \n", tempstock);

        price_t tempprice = user(qty_len + price_len - 1, price_len);
        printf("price: [%d] \n", tempprice);

        qty_t tempqty = user(qty_len - 1, 0);
        printf("qty: [%d] \n", tempqty);

        // money = tempqty * tempprice;
        // printf("money: [%d] \n", money);

        qty.write(tempqty);
        price.write(tempprice);
        stock.write(tempstock);
    }
}

void dataassign2(int *data, hls::stream<int> &streamdata)
{
    std::cout << "!!!Start read!!!" << std::endl;
    for (int i = 0; i < num_users; i++)
    {
#pragma HLS LOOP_TRIPCOUNT min = size max = size
        // Blocking write command to inStream
        std::cout << "krnl read=" << data[i] << std::endl;
        streamdata << data[i];
    }
    std::cout << "!!!End read!!!" << std::endl;
}

void checkStock(hls::stream<stock_t> &stock, hls::stream<int> &DB_stock, hls::stream<result_t> &check_stock_out)
{
    for (int i = 0; i < num_users; i++)
    {
        stock_t s = stock.read();
        int dbstock = DB_stock.read();

        result_t result;
        result = 0;
        printf("stock: %d, 結果: %d stock: %d\n", s, result, dbstock);
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

void checkPrice(hls::stream<price_t> &price, hls::stream<qty_t> &qty, hls::stream<int> &DB_account, hls::stream<result_t> &check_price_out)
{
    for (int i = 0; i < num_users; i++)
    {
        price_t p = price.read();
        qty_t q = qty.read();
        int amt = DB_account.read();
        result_t result;
        if (p > price_limit)
        {
            result = 1;
        }
        else
        {
            result = 0;
        }
        printf("price: %d, qty: %d amt: %d\n", p, q, amt);
        check_price_out.write(result);
    }
}