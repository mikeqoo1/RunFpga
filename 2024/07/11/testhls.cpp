#include "hls.hpp"

struct Order
{
    int stockno;
    int price;
    int qty;
    int bs;
};

struct Limit
{
    int stockno;
    int qty;
};

int main()
{
    order_t all_order[customers_max];
    Order orderlist[customers_max];
    Limit limitlist[customers_max];
    for (int i = 0; i < customers_max; i++)
    {
        order_t order;
        Order o;
        Limit l;
        int stock = 0;
        int price = 0;
        int qty = 0;
        int bs = 0;
        if (i == 0)
        {
            stock = 50;
            price = 10;
            qty = 1;
            bs = 1;
            o.stockno = stock;
            o.price = price;
            o.qty = qty;
            o.bs = bs;
            order(order_len - 1, price_len + qty_len + bs_len) = stock;
            order(price_len + qty_len + bs_len - 1, price_len) = price;
            order(qty_len + bs_len - 1, qty_len) = qty;
            order(bs_len - 1, 0) = bs;
            l.stockno = stock;
            l.qty = qty;
        }
        else if (i == 1)
        {
            stock = 6016;
            price = 20;
            qty = 5;
            bs = 1;
            o.stockno = stock;
            o.price = price;
            o.qty = qty;
            o.bs = bs;
            order(order_len - 1, price_len + qty_len + bs_len) = stock;
            order(price_len + qty_len + bs_len - 1, price_len) = price;
            order(qty_len + bs_len - 1, qty_len) = qty;
            order(bs_len - 1, 0) = bs;
            l.stockno = stock;
            l.qty = qty;
        }
        else if (i == 2)
        {
            stock = 2330;
            price = 500;
            qty = 3;
            bs = 1;
            o.stockno = stock;
            o.price = price;
            o.qty = qty;
            o.bs = bs;
            order(order_len - 1, price_len + qty_len + bs_len) = stock;
            order(price_len + qty_len + bs_len - 1, price_len) = price;
            order(qty_len + bs_len - 1, qty_len) = qty;
            order(bs_len - 1, 0) = bs;
            l.stockno = stock;
            l.qty = qty;
        }
        else if (i == 3)
        {
            stock = 1416;
            price = 50;
            qty = 5;
            bs = 1;
            o.stockno = stock;
            o.price = price;
            o.qty = qty;
            o.bs = bs;
            order(order_len - 1, price_len + qty_len + bs_len) = stock;
            order(price_len + qty_len + bs_len - 1, price_len) = price;
            order(qty_len + bs_len - 1, qty_len) = qty;
            order(bs_len - 1, 0) = bs;
            l.stockno = stock;
            l.qty = qty;
        }
        else if (i == 4)
        {
            stock = 1234;
            price = 58;
            qty = 3;
            bs = 1;
            o.stockno = stock;
            o.price = price;
            o.qty = qty;
            o.bs = bs;
            order(order_len - 1, price_len + qty_len + bs_len) = stock;
            order(price_len + qty_len + bs_len - 1, price_len) = price;
            order(qty_len + bs_len - 1, qty_len) = qty;
            order(bs_len - 1, 0) = bs;
            l.stockno = stock;
            l.qty = qty;
        }
        else if (i == 5)
        {
            stock = 939;
            price = 15;
            qty = 200;
            bs = 2;
            o.stockno = stock;
            o.price = price;
            o.qty = qty;
            o.bs = bs;
            order(order_len - 1, price_len + qty_len + bs_len) = stock;
            order(price_len + qty_len + bs_len - 1, price_len) = price;
            order(qty_len + bs_len - 1, qty_len) = qty;
            order(bs_len - 1, 0) = bs;
            l.stockno = stock;
            l.qty = qty;
        }
        else if (i == 6)
        {
            stock = 2435;
            price = 1000;
            qty = 10;
            bs = 2;
            o.stockno = stock;
            o.price = price;
            o.qty = qty;
            o.bs = bs;
            order(order_len - 1, price_len + qty_len + bs_len) = stock;
            order(price_len + qty_len + bs_len - 1, price_len) = price;
            order(qty_len + bs_len - 1, qty_len) = qty;
            order(bs_len - 1, 0) = bs;
            l.stockno = stock;
            l.qty = qty;
        }
        else if (i == 7)
        {
            stock = 1056;
            price = 11;
            qty = 50;
            bs = 2;
            o.stockno = stock;
            o.price = price;
            o.qty = qty;
            o.bs = bs;
            order(order_len - 1, price_len + qty_len + bs_len) = stock;
            order(price_len + qty_len + bs_len - 1, price_len) = price;
            order(qty_len + bs_len - 1, qty_len) = qty;
            order(bs_len - 1, 0) = bs;
            l.stockno = stock;
            l.qty = qty;
        }
        else if (i == 8)
        {
            stock = 7058;
            price = 55;
            qty = 10;
            bs = 2;
            o.stockno = stock;
            o.price = price;
            o.qty = qty;
            o.bs = bs;
            order(order_len - 1, price_len + qty_len + bs_len) = stock;
            order(price_len + qty_len + bs_len - 1, price_len) = price;
            order(qty_len + bs_len - 1, qty_len) = qty;
            order(bs_len - 1, 0) = bs;
            l.stockno = stock;
            l.qty = qty;
        }
        else if (i == 9)
        {
            stock = 2589;
            price = 22;
            qty = 66;
            bs = 2;
            o.stockno = stock;
            o.price = price;
            o.qty = qty;
            o.bs = bs;
            order(order_len - 1, price_len + qty_len + bs_len) = stock;
            order(price_len + qty_len + bs_len - 1, price_len) = price;
            order(qty_len + bs_len - 1, qty_len) = qty;
            order(bs_len - 1, 0) = bs;
            l.stockno = stock;
            l.qty = qty;
        }
        else
        {
            o.stockno = stock;
            o.price = price;
            o.qty = qty;
            o.bs = bs;
            order(order_len - 1, price_len + qty_len + bs_len) = stock;
            order(price_len + qty_len + bs_len - 1, price_len) = price;
            order(qty_len + bs_len - 1, qty_len) = qty;
            order(bs_len - 1, 0) = bs;
            l.stockno = stock;
            l.qty = qty;
        }

        // 從 user 中拿出 stock price 和 qty
        int tempstock = order(stock_len + price_len + qty_len + bs_len - 1, price_len + qty_len + bs_len);
        int tempprice = order(price_len + qty_len + bs_len - 1, price_len);
        int tempqty = order(qty_len + bs_len - 1, qty_len);
        int tempbs = order(bs_len - 1, 0);
        if (i == 0 || i == 1 || i == 2 || i == 3 || i == 4 || i == 5 || i == 6 || i == 7 || i == 8 || i == 9)
        {
            printf("Order    i=%d, order:%d, stock:%d, price:%d, qty:%d, bs:%d\n", i, order, tempstock, tempprice, tempqty, tempbs);
            // int ddd = db(db_stock_len - 1, qty_len);
            // int bbb = db(qty_len - 1, 0);
            // printf("DB_Stock i=%d, db:%d, stock:%d, qty:%d\n", i, db, ddd, bbb);
        }
        all_order[i] = order;
        orderlist[i] = o;
        limitlist[i] = l;
    }

    int out[customers_max] = {0};
    riskcontrol(all_order, out, customers_max);
    int sw_use_amt = 0;
    int money = 0;
    for (int i = 0; i < customers_max; i++)
    {
        int swans;
        if (orderlist[i].bs == 1)
        {
            money = orderlist[i].qty * orderlist[i].price * 1000;
            sw_use_amt += money;
            printf("SW_單筆價金 i=%d 已用投資額度 i=%d \n", money, sw_use_amt);
            if (5000000 < sw_use_amt)
            {
                swans = 1;
            }
            else
            {
                swans = 0;
            }
        }
        else if (orderlist[i].bs == 2)
        {
            if (limitlist[i].qty < orderlist[i].qty)
            {
                swans = 1;
            }
            else
            {
                swans = 0;
            }
        }
        else
        {
            swans = 0;
        }

        if (out[i] != swans)
        {
            printf("仿真模擬失敗i=%d 軟體結果:%d 硬體結果:%d\n", i, swans, out[i]);
        }
    }

    uint32_t IPVal = 0X0A0C0300;
    uint8_t ip[4];
    *(uint32_t *)ip = IPVal;
    printf("ip %d.%d.%d.%d\n", ip[0], ip[1], ip[2], ip[3]);
    return 0;
}
