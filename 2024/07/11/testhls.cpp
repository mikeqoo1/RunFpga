#include "hls.hpp"

int main()
{
    int amt, skqty;
    amt = 5000000;
    skqty = 999;

    order_obj all_order[num_users];
    hls::stream<order_t> all_user_s;
    hls::stream<db_stock_t> all_stock;
    for (int i = 0; i < num_users; i++)
    {
        order_t user;
        db_stock_t db;
        order_obj user_sw;
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
            user_sw.stock = stock;
            user_sw.price = price;
            user_sw.qty = qty;
            user_sw.bs = bs;
            user(order_len - 1, price_len + qty_len + bs_len) = stock;
            user(price_len + qty_len + bs_len - 1, price_len) = price;
            user(qty_len + bs_len - 1, qty_len) = qty;
            user(bs_len - 1, 0) = bs;
            db(db_stock_len - 1, qty_len) = stock;
            db(qty_len - 1, 0) = qty;
        }
        else if (i == 1)
        {
            stock = 6016;
            price = 20;
            qty = 5;
            bs = 1;
            user_sw.stock = stock;
            user_sw.price = price;
            user_sw.qty = qty;
            user_sw.bs = bs;
            user(order_len - 1, price_len + qty_len + bs_len) = stock;
            user(price_len + qty_len + bs_len - 1, price_len) = price;
            user(qty_len + bs_len - 1, qty_len) = qty;
            user(bs_len - 1, 0) = bs;
            db(db_stock_len - 1, qty_len) = stock;
            db(qty_len - 1, 0) = qty;
        }
        else if (i == 2)
        {
            stock = 2330;
            price = 500;
            qty = 3;
            bs = 1;
            user_sw.stock = stock;
            user_sw.price = price;
            user_sw.qty = qty;
            user_sw.bs = bs;
            user(order_len - 1, price_len + qty_len + bs_len) = stock;
            user(price_len + qty_len + bs_len - 1, price_len) = price;
            user(qty_len + bs_len - 1, qty_len) = qty;
            user(bs_len - 1, 0) = bs;
            db(db_stock_len - 1, qty_len) = stock;
            db(qty_len - 1, 0) = qty;
        }
        else if (i == 3)
        {
            stock = 1416;
            price = 50;
            qty = 5;
            bs = 1;
            user_sw.stock = stock;
            user_sw.price = price;
            user_sw.qty = qty;
            user_sw.bs = bs;
            user(order_len - 1, price_len + qty_len + bs_len) = stock;
            user(price_len + qty_len + bs_len - 1, price_len) = price;
            user(qty_len + bs_len - 1, qty_len) = qty;
            user(bs_len - 1, 0) = bs;
            db(db_stock_len - 1, qty_len) = stock;
            db(qty_len - 1, 0) = qty;
        }
        else if (i == 4)
        {
            stock = 1234;
            price = 58;
            qty = 3;
            bs = 1;
            user_sw.stock = stock;
            user_sw.price = price;
            user_sw.qty = qty;
            user_sw.bs = bs;
            user(order_len - 1, price_len + qty_len + bs_len) = stock;
            user(price_len + qty_len + bs_len - 1, price_len) = price;
            user(qty_len + bs_len - 1, qty_len) = qty;
            user(bs_len - 1, 0) = bs;
            db(db_stock_len - 1, qty_len) = stock;
            db(qty_len - 1, 0) = qty;
        }
        else if (i == 5)
        {
            stock = 939;
            price = 15;
            qty = 200;
            bs = 2;
            user_sw.stock = stock;
            user_sw.price = price;
            user_sw.qty = qty;
            user_sw.bs = bs;
            user(order_len - 1, price_len + qty_len + bs_len) = stock;
            user(price_len + qty_len + bs_len - 1, price_len) = price;
            user(qty_len + bs_len - 1, qty_len) = qty;
            user(bs_len - 1, 0) = bs;
            db(db_stock_len - 1, qty_len) = stock;
            db(qty_len - 1, 0) = qty;
        }
        else if (i == 6)
        {
            stock = 2435;
            price = 1000;
            qty = 10;
            bs = 2;
            user_sw.stock = stock;
            user_sw.price = price;
            user_sw.qty = qty;
            user_sw.bs = bs;
            user(order_len - 1, price_len + qty_len + bs_len) = stock;
            user(price_len + qty_len + bs_len - 1, price_len) = price;
            user(qty_len + bs_len - 1, qty_len) = qty;
            user(bs_len - 1, 0) = bs;
            db(db_stock_len - 1, qty_len) = stock;
            db(qty_len - 1, 0) = qty;
        }
        else if (i == 7)
        {
            stock = 1056;
            price = 11;
            qty = 50;
            bs = 2;
            user_sw.stock = stock;
            user_sw.price = price;
            user_sw.qty = qty;
            user_sw.bs = bs;
            user(order_len - 1, price_len + qty_len + bs_len) = stock;
            user(price_len + qty_len + bs_len - 1, price_len) = price;
            user(qty_len + bs_len - 1, qty_len) = qty;
            user(bs_len - 1, 0) = bs;
            db(db_stock_len - 1, qty_len) = stock;
            db(qty_len - 1, 0) = qty;
        }
        else if (i == 8)
        {
            stock = 7058;
            price = 55;
            qty = 10;
            bs = 2;
            user_sw.stock = stock;
            user_sw.price = price;
            user_sw.qty = qty;
            user_sw.bs = bs;
            user(order_len - 1, price_len + qty_len + bs_len) = stock;
            user(price_len + qty_len + bs_len - 1, price_len) = price;
            user(qty_len + bs_len - 1, qty_len) = qty;
            user(bs_len - 1, 0) = bs;
            db(db_stock_len - 1, qty_len) = stock;
            db(qty_len - 1, 0) = qty;
        }
        else if (i == 9)
        {
            stock = 2589;
            price = 22;
            qty = 66;
            bs = 2;
            user_sw.stock = stock;
            user_sw.price = price;
            user_sw.qty = qty;
            user_sw.bs = bs;
            user(order_len - 1, price_len + qty_len + bs_len) = stock;
            user(price_len + qty_len + bs_len - 1, price_len) = price;
            user(qty_len + bs_len - 1, qty_len) = qty;
            user(bs_len - 1, 0) = bs;
            db(db_stock_len - 1, qty_len) = stock;
            db(qty_len - 1, 0) = qty;
        } else {
            user_sw.stock = stock;
            user_sw.price = price;
            user_sw.qty = qty;
            user_sw.bs = bs;
            user(order_len - 1, price_len + qty_len + bs_len) = stock;
            user(price_len + qty_len + bs_len - 1, price_len) = price;
            user(qty_len + bs_len - 1, qty_len) = qty;
            user(bs_len - 1, 0) = bs;
            db(db_stock_len - 1, qty_len) = stock;
            db(qty_len - 1, 0) = qty;
        }

        // 從 user 中拿出 stock price 和 qty
        int tempstock = user(stock_len + price_len + qty_len + bs_len - 1, price_len + qty_len + bs_len);
        int tempprice = user(price_len + qty_len + bs_len - 1, price_len);
        int tempqty = user(qty_len + bs_len - 1, qty_len);
        int tempbs = user(bs_len - 1, 0);
        if (i == 0)
        {
            printf("Order    i=%d, user:%d, stock:%d, price:%d, qty:%d, bs:%d\n", i, user, tempstock, tempprice, tempqty, tempbs);
            int ddd = db(db_stock_len - 1, qty_len);
            int bbb = db(qty_len - 1, 0);
            printf("DB_Stock i=%d, db:%d, stock:%d, qty:%d\n", i, db, ddd, bbb);
        }
        all_order[i] = user_sw;
        all_user_s.write(user);
        all_stock.write(db);
    }

    hls::stream<result_t> qty_sw;
    hls::stream<result_t> price_sw;
    for (int i = 0; i < num_users; i++)
    {
        result_t result;
        order_obj user_sw = all_order[i];
        // printf("i = %d; stock: %d; price:%d, qty:%d\n", i, user_sw.stock, user_sw.qty, user_sw.price);
        if (user_sw.bs == 1)
        {
            result = 0;
            qty_sw.write(result);
            if (user_sw.price * user_sw.qty > amt)
            {
                result = 1;
                price_sw.write(result);
            }
            else
            {
                result = 0;
                price_sw.write(result);
            }
        }
        else
        {
            result = 0;
            price_sw.write(result);
            if (user_sw.qty > skqty)
            {
                result = 1;
                qty_sw.write(result);
            }
            else
            {
                result = 0;
                qty_sw.write(result);
            }
        }
    }

    // hardware
    // hls::stream<result_t> qty;
    // hls::stream<result_t> price;
    topfunc(all_order);
    // riskcontrol(all_user_s, all_stock, qty, price);

    // check hardware and software
    // for (int i = 0; i < num_users; i++)
    // {
    //     if (qty.read() != qty_sw.read())
    //     {
    //         cout << "數量不一樣" << qty.read() << "!?!?!" << qty_sw.read() << endl;
    //     }

    //     if (price.read() != price_sw.read())
    //     {
    //         cout << "價錢不一樣" << price.read() << "!?!?!" << price_sw.read() << endl;
    //     }
    // }
    uint32_t IPVal = 0X0A0C0300;
    uint8_t ip[4];
    *(uint32_t *)ip = IPVal;
    printf("ip %d.%d.%d.%d\n", ip[0], ip[1], ip[2], ip[3]);
    return 0;
}
