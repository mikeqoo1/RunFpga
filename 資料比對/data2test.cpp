#include "data2.h"

int main()
{
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
        user(stock_len - 1, 0) = (stock_t)stock;
        user = user << price_len;

        int price = rand() % 9999;
        // cout << "price=" << price << endl;
        user_sw.price = price;
        // 把 price 存入 user
        user(price_len - 1, 0) = (price_t)price;
        user = user << qty_len;

        int qty = rand() % 16384;
        // cout << "qty=" << qty << endl;
        user_sw.qty = qty;
        // 把 qty 存入 user
        user(qty_len - 1, 0) = (qty_t)qty;

        printf("i = %d; stock: %d; price:%d, qty:%d\n", i, (int)user(price_len - 1, qty_len), (int)user(price_len - 1, qty_len), (int)user(qty_len - 1, 0));
        all_user[i] = user_sw;
        all_user_s.write(user);
    }

    // hardware
    riskcontrol(all_user_s);

    return 0;
}
