#include "hls_task.h"
#include <iostream>
#include <stdio.h>
#include <ap_int.h>
#include <hls_stream.h>

using namespace std;
#define num_users 100
#define price_limit 5000000
#define qty_limit 999
#define stock_len 14
#define qty_len 14
#define price_len 20
#define user_len stock_len + qty_len + price_len

typedef ap_uint<user_len> user_t;
typedef ap_uint<stock_len> stock_t;
typedef ap_uint<qty_len> qty_t;
typedef ap_uint<price_len> price_t;
typedef ap_uint<2> result_t;
struct user_obj
{
    int stock;
    int price;
    int qty;
};

// Top function
void riskcontrol(hls::stream<user_t> &inputdata, int *account, int *stock, hls::stream<result_t> &stock_out, hls::stream<result_t> &qty_out, hls::stream<result_t> &price_out);
// Sub function
void dataassign(hls::stream<user_t> &in, hls::stream<stock_t> &stock, hls::stream<qty_t> &qty, hls::stream<price_t> &price);
void checkStock(hls::stream<stock_t> &stock, hls::stream<int> &DB_stock, hls::stream<result_t> &Stockout);
void checkQty(hls::stream<qty_t> &qty, hls::stream<result_t> &Qtyout);
void checkPrice(hls::stream<price_t> &price, hls::stream<qty_t> &qty, hls::stream<int> &DB_account, hls::stream<result_t> &Priceout);

void dataassign2(int *data, hls::stream<int> &streamdata);