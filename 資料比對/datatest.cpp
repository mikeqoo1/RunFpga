#include "data.h"

int main()
{
    user_obj all_user[num_users];
    hls::stream<user_t> all_user_s;
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

        printf("i = %d; stock: %d; price:%d, qty:%d\n", i, stock, price, qty);
        all_user[i] = user_sw;
        all_user_s.write(user);
    }

    // hardware
    riskcontrol(all_user_s);

    return 0;
}
