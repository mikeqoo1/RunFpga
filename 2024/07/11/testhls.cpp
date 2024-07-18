#include "hls.hpp"

int main()
{
    order_t all_order[customers_max];
    for (int i = 0; i < customers_max; i++)
    {
        order_t order;
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
            // user_sw.stock = stock;
            // user_sw.price = price;
            // user_sw.qty = qty;
            // user_sw.bs = bs;
            order(order_len - 1, price_len + qty_len + bs_len) = stock;
            order(price_len + qty_len + bs_len - 1, price_len) = price;
            order(qty_len + bs_len - 1, qty_len) = qty;
            order(bs_len - 1, 0) = bs;
            // db(db_stock_len - 1, qty_len) = stock;
            // db(qty_len - 1, 0) = qty;
        }
        else if (i == 1)
        {
            stock = 6016;
            price = 20;
            qty = 5;
            bs = 1;
            // user_sw.stock = stock;
            // user_sw.price = price;
            // user_sw.qty = qty;
            // user_sw.bs = bs;
            order(order_len - 1, price_len + qty_len + bs_len) = stock;
            order(price_len + qty_len + bs_len - 1, price_len) = price;
            order(qty_len + bs_len - 1, qty_len) = qty;
            order(bs_len - 1, 0) = bs;
            // db(db_stock_len - 1, qty_len) = stock;
            // db(qty_len - 1, 0) = qty;
        }
        else if (i == 2)
        {
            stock = 2330;
            price = 500;
            qty = 3;
            bs = 1;
            // user_sw.stock = stock;
            // user_sw.price = price;
            // user_sw.qty = qty;
            // user_sw.bs = bs;
            order(order_len - 1, price_len + qty_len + bs_len) = stock;
            order(price_len + qty_len + bs_len - 1, price_len) = price;
            order(qty_len + bs_len - 1, qty_len) = qty;
            order(bs_len - 1, 0) = bs;
            // db(db_stock_len - 1, qty_len) = stock;
            // db(qty_len - 1, 0) = qty;
        }
        else if (i == 3)
        {
            stock = 1416;
            price = 50;
            qty = 5;
            bs = 1;
            // user_sw.stock = stock;
            // user_sw.price = price;
            // user_sw.qty = qty;
            // user_sw.bs = bs;
            order(order_len - 1, price_len + qty_len + bs_len) = stock;
            order(price_len + qty_len + bs_len - 1, price_len) = price;
            order(qty_len + bs_len - 1, qty_len) = qty;
            order(bs_len - 1, 0) = bs;
            // db(db_stock_len - 1, qty_len) = stock;
            // db(qty_len - 1, 0) = qty;
        }
        else if (i == 4)
        {
            stock = 1234;
            price = 58;
            qty = 3;
            bs = 1;
            // user_sw.stock = stock;
            // user_sw.price = price;
            // user_sw.qty = qty;
            // user_sw.bs = bs;
            order(order_len - 1, price_len + qty_len + bs_len) = stock;
            order(price_len + qty_len + bs_len - 1, price_len) = price;
            order(qty_len + bs_len - 1, qty_len) = qty;
            order(bs_len - 1, 0) = bs;
            // db(db_stock_len - 1, qty_len) = stock;
            // db(qty_len - 1, 0) = qty;
        }
        else if (i == 5)
        {
            stock = 939;
            price = 15;
            qty = 200;
            bs = 2;
            // user_sw.stock = stock;
            // user_sw.price = price;
            // user_sw.qty = qty;
            // user_sw.bs = bs;
            order(order_len - 1, price_len + qty_len + bs_len) = stock;
            order(price_len + qty_len + bs_len - 1, price_len) = price;
            order(qty_len + bs_len - 1, qty_len) = qty;
            order(bs_len - 1, 0) = bs;
            // db(db_stock_len - 1, qty_len) = stock;
            // db(qty_len - 1, 0) = qty;
        }
        else if (i == 6)
        {
            stock = 2435;
            price = 1000;
            qty = 10;
            bs = 2;
            // user_sw.stock = stock;
            // user_sw.price = price;
            // user_sw.qty = qty;
            // user_sw.bs = bs;
            order(order_len - 1, price_len + qty_len + bs_len) = stock;
            order(price_len + qty_len + bs_len - 1, price_len) = price;
            order(qty_len + bs_len - 1, qty_len) = qty;
            order(bs_len - 1, 0) = bs;
            // db(db_stock_len - 1, qty_len) = stock;
            // db(qty_len - 1, 0) = qty;
        }
        else if (i == 7)
        {
            stock = 1056;
            price = 11;
            qty = 50;
            bs = 2;
            // user_sw.stock = stock;
            // user_sw.price = price;
            // user_sw.qty = qty;
            // user_sw.bs = bs;
            order(order_len - 1, price_len + qty_len + bs_len) = stock;
            order(price_len + qty_len + bs_len - 1, price_len) = price;
            order(qty_len + bs_len - 1, qty_len) = qty;
            order(bs_len - 1, 0) = bs;
            // db(db_stock_len - 1, qty_len) = stock;
            // db(qty_len - 1, 0) = qty;
        }
        else if (i == 8)
        {
            stock = 7058;
            price = 55;
            qty = 10;
            bs = 2;
            // user_sw.stock = stock;
            // user_sw.price = price;
            // user_sw.qty = qty;
            // user_sw.bs = bs;
            order(order_len - 1, price_len + qty_len + bs_len) = stock;
            order(price_len + qty_len + bs_len - 1, price_len) = price;
            order(qty_len + bs_len - 1, qty_len) = qty;
            order(bs_len - 1, 0) = bs;
            // db(db_stock_len - 1, qty_len) = stock;
            // db(qty_len - 1, 0) = qty;
        }
        else if (i == 9)
        {
            stock = 2589;
            price = 22;
            qty = 66;
            bs = 2;
            // user_sw.stock = stock;
            // user_sw.price = price;
            // user_sw.qty = qty;
            // user_sw.bs = bs;
            order(order_len - 1, price_len + qty_len + bs_len) = stock;
            order(price_len + qty_len + bs_len - 1, price_len) = price;
            order(qty_len + bs_len - 1, qty_len) = qty;
            order(bs_len - 1, 0) = bs;
            // db(db_stock_len - 1, qty_len) = stock;
            // db(qty_len - 1, 0) = qty;
        }
        else
        {
            // user_sw.stock = stock;
            // user_sw.price = price;
            // user_sw.qty = qty;
            // user_sw.bs = bs;
            order(order_len - 1, price_len + qty_len + bs_len) = stock;
            order(price_len + qty_len + bs_len - 1, price_len) = price;
            order(qty_len + bs_len - 1, qty_len) = qty;
            order(bs_len - 1, 0) = bs;
            // db(db_stock_len - 1, qty_len) = stock;
            // db(qty_len - 1, 0) = qty;
        }

        // 從 user 中拿出 stock price 和 qty
        int tempstock = order(stock_len + price_len + qty_len + bs_len - 1, price_len + qty_len + bs_len);
        int tempprice = order(price_len + qty_len + bs_len - 1, price_len);
        int tempqty = order(qty_len + bs_len - 1, qty_len);
        int tempbs = order(bs_len - 1, 0);
        if (i == 0)
        {
            printf("Order    i=%d, order:%d, stock:%d, price:%d, qty:%d, bs:%d\n", i, order, tempstock, tempprice, tempqty, tempbs);
            // int ddd = db(db_stock_len - 1, qty_len);
            // int bbb = db(qty_len - 1, 0);
            // printf("DB_Stock i=%d, db:%d, stock:%d, qty:%d\n", i, db, ddd, bbb);
        }
        all_order[i] = order;
    }

    int out[customers_max] = {0};
    riskcontrol(all_order, out, customers_max);
    for (int i = 0; i < customers_max; i++)
    {
        printf("%d\n", out[i]);
    }

    uint32_t IPVal = 0X0A0C0300;
    uint8_t ip[4];
    *(uint32_t *)ip = IPVal;
    printf("ip %d.%d.%d.%d\n", ip[0], ip[1], ip[2], ip[3]);
    return 0;
}
