#include "hls_task.h"
#include <iostream>
#include <stdio.h>
#include <ap_int.h>
#include <hls_stream.h>

using namespace std;
#define limit_amt 5000000
#define num_users 128
#define stock_len 14
#define price_len 20
#define qty_len 14
#define bs_len 2
#define order_len stock_len + price_len + qty_len + bs_len
#define db_stock_len stock_len + qty_len

typedef ap_uint<order_len> order_t;
typedef ap_uint<stock_len> stock_t;
typedef ap_uint<price_len> price_t;
typedef ap_uint<qty_len> qty_t;
typedef ap_uint<bs_len> bs_t;
typedef ap_uint<2> result_t;
typedef ap_uint<db_stock_len> db_stock_t;
struct order_obj
{
    int stock;
    int price;
    int qty;
    int bs;
};
// Top function
void topfunc(order_obj *orderlist);
// Sub function
void riskcontrol(hls::stream<order_t> &inputdata, hls::stream<db_stock_t> &db, hls::stream<result_t> &qty_out, hls::stream<result_t> &price_out);
void dataassign(hls::stream<order_t> &in, hls::stream<stock_t> &stock, hls::stream<price_t> &price, hls::stream<qty_t> &qty, hls::stream<bs_t> &bs, hls::stream<int> &use_amt);
void getinventory(hls::stream<db_stock_t> &db_stock_qty, hls::stream<stock_t> &db_stock, hls::stream<qty_t> &db_qty);
void checkQty(hls::stream<qty_t> &qty, hls::stream<stock_t> &stock, hls::stream<stock_t> &db_stock, hls::stream<qty_t> &db_qty, hls::stream<result_t> &Qtyout, hls::stream<result_t> &Priceout);
void checkPrice(hls::stream<price_t> &price, hls::stream<int> &use_amt, hls::stream<result_t> &Priceout, hls::stream<result_t> &Qtyout);
