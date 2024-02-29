#include "ace_kernel.h"

int main()
{
    // 家銘版本
    user_obj all_user[num_users];
    hls::stream<user_t> all_user_s;
    printf("%d,\n", 8 << 1);
    for (int i = 0; i < num_users; i++)
    {
        user_t user;
        user_obj user_sw;
        int stock = rand() % 16384;
        user_sw.stock = stock;
        user(stock_len - 1, 0) = (stock_t)stock;
        user = user << price_len;

        int price = rand() % 9999;
        user_sw.price = price;
        user(price_len - 1, 0) = (price_t)price;
        user = user << qty_len;

        int qty = rand() % 16384;
        user_sw.qty = qty;
        user(qty_len - 1, 0) = (qty_t)qty;

        printf("i = %d; stock: %d; price:%d, qty:%d\n", i, (int)user(price_len - 1, qty_len), (int)user(price_len - 1, qty_len), (int)user(qty_len - 1, 0));
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
        printf("i = %d; stock: %d; price:%d, qty:%d\n", i, user_sw.stock, user_sw.qty, user_sw.price);
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

        if (user_sw.price * user_sw.qty > price_limit)
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
