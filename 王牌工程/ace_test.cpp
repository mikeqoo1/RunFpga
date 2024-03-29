#include "ace_kernel.h"

int main()
{
    // 家銘版本
    user_obj all_user[num_users];
    hls::stream<user_t> all_user_s;
    for (int i = 0; i < num_users; i++)
    {
        user_t user;
        user_obj user_sw;
        int stock = rand() % 16384;
        // cout << "stock=" << stock << endl;
        user_sw.stock = stock;
        // 把 stock 存入 user
        user(user_len - 1, qty_len + price_len) = stock;

        int price = rand() % 9999;
        // cout << "price=" << price << endl;
        user_sw.price = price;
        // 把 price 存入 user
        user(qty_len + price_len - 1, price_len) = price;

        int qty = rand() % 16384;
        // cout << "qty=" << qty << endl;
        user_sw.qty = qty;
        // 把 qty 存入 user
        user(price_len - 1, 0) = qty;
        // 從 user 中拿出 stock price 和 qty
        int tempstock = user(stock_len + qty_len + price_len - 1, qty_len + price_len);
        int tempprice = user(qty_len + price_len - 1, price_len);
        int tempqty = user(qty_len - 1, 0);

        // printf("i = %d; stock: %d; price:%d, qty:%d\n", i, tempstock, tempprice, tempqty);
        all_user[i] = user_sw;
        all_user_s.write(user);
    }

    hls::stream<result_t> stock_sw;
    hls::stream<result_t> qty_sw;
    hls::stream<result_t> price_sw;
    for (int i = 0; i < num_users; i++)
    {
        result_t result;
        user_obj user_sw = all_user[i];
        // printf("i = %d; stock: %d; price:%d, qty:%d\n", i, user_sw.stock, user_sw.qty, user_sw.price);
        result = 0;
        stock_sw.write(result);
        if (user_sw.qty > qty_limit)
        {
            result = 1;
            qty_sw.write(result);
        }
        else
        {
            result = 0;
            qty_sw.write(result);
        }

        if (user_sw.price > price_limit)
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

    // hardware
    hls::stream<result_t> stock;
    hls::stream<result_t> qty;
    hls::stream<result_t> price;
    riskcontrol(all_user_s, stock, qty, price);

    // check hardware and software
    for (int i = 0; i < num_users; i++)
    {
        if (stock.read() != stock_sw.read())
        {
            cout << "股票不一樣" << stock.read() << "!?!?!" << stock_sw.read() << endl;
        }

        if (qty.read() != qty_sw.read())
        {
            cout << "數量不一樣" << qty.read() << "!?!?!" << qty_sw.read() << endl;
        }

        if (price.read() != price_sw.read())
        {
            cout << "價錢不一樣" << price.read() << "!?!?!" << price_sw.read() << endl;
        }
    }

    return 0;
}
