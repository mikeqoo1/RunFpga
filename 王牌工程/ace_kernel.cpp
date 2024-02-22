#include "ace_kernel.h"

/* TASK 版本  */
const int UNIT_QTY = 1000; // 數量的單位
const int UNIT_PRICE = 3;  // 價格的單位
void dataassign(hls::stream<int> &in,
                hls::stream<int> &stock, hls::stream<int> &qty, hls::stream<int> &price)
{
    static int tempqty = 0;
    int data = in.read();
    //std::cout << "資料是" << data << std::endl;
    if (data % UNIT_QTY == 0) // 數量都要以 1000 為單位
    {
        tempqty = data;
        qty.write(data);
    }
    else if (data % UNIT_PRICE == 0) // 價錢都是以 3 為單位
    {
        int money = tempqty * data;
        //std::cout << "價金是" << money << std::endl;
        price.write(money);
    }
    else
    {
        stock.write(data);
    }
}

void checkF15(hls::stream<int> &qty, hls::stream<int> &f15out)
{
    int inv = qty.read();
    //std::cout << "庫存是" << inv << std::endl;
    if (inv > 30000)
        f15out.write(1111);
    else
        f15out.write(0);
}

void checkF16(hls::stream<int> &stock, hls::stream<int> &f16out)
{
    int id = stock.read();
    //std::cout << "股票代號是" << id << std::endl;
    f16out.write(0);
}

void checkF22(hls::stream<int> &price, hls::stream<int> &f22out)
{
    int money = price.read();
    //std::cout << "價金是" << money << std::endl;
    if (money > 5000000)
        f22out.write(1234);
    else
        f22out.write(0);
}

void riskcontrol(hls::stream<int> &inputdata,
                 hls::stream<int> &F15, hls::stream<int> &F16, hls::stream<int> &F22)
{
    // 本地端的數據流 stockid 和 qty 和 price (也就是 task 的輸入輸出通道) 以及 task 本身都要聲明為 hls_thread_local, 保證 TopFunction 呼叫時是活的
    hls_thread_local hls::stream<int, 3> stockid;
    hls_thread_local hls::stream<int, 3> qty;
    hls_thread_local hls::stream<int, 3> price;
    hls_thread_local hls::task t0 (dataassign, inputdata, stockid, qty, price); // 處理資料
    hls_thread_local hls::task t1 (checkF15, qty, F15);                         // 檢查庫存
    hls_thread_local hls::task t2 (checkF16, stockid, F16);                     // 檢查停止融資券
    hls_thread_local hls::task t3 (checkF22, price, F22);                       // 檢查額度
}

/*DATAFLOW 版本 */
/*
void riskcontrol(int In[9], int F15Out[3], int F16Out[3], int F22Out[3])
{
    int stockid[3], qty[3], price[3];
#pragma HLS dataflow
    dataassign(In, stockid, qty, price);
    checkF15(qty, F15Out);
    checkF16(stockid, F16Out);
    checkF22(price, F22Out);
}

// Sub functions
void dataassign(int *in, int *stock, int *qty, int *price)
{
#pragma HLS inline off
    for (int i = 0; i < 3; ++i)
    {
        stock[i] = in[i * 3];
        qty[i] = in[i * 3 + 1];
        int qtyyyy = in[i * 3 + 1];
        int priceeee = in[i * 3 + 2];
        int money = qtyyyy * priceeee;
        price[i] = money;
    }
}
void checkF15(int *qty, int *f15out)
{
#pragma HLS inline off
    for (int i = 0; i < 3; ++i)
    {
        if (qty[i] > 30000)
            f15out[i] = 1111;
        else
            f15out[i] = 0;
    }
}
void checkF16(int *stock, int *f16out)
{
#pragma HLS inline off
    for (int i = 0; i < 3; ++i)
    {
        f16out[i] = stock[i];
    }
}
void checkF22(int *price, int *f22out)
{
#pragma HLS inline off
    for (int i = 0; i < 3; ++i)
    {
        if (price[i] > 5000000)
            f22out[i] = 1234;
        else
            f22out[i] = 0;
    }
}
*/