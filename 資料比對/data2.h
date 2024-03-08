#include "hls_task.h"
#include <iostream>
#include <stdio.h>
#include <ap_int.h>
#include <hls_stream.h>

using namespace std;
#define num_users 100
#define stock_len 14
#define qty_len 14
#define price_len 20
#define user_len stock_len + qty_len + price_len

typedef ap_uint<user_len> user_t;
typedef ap_uint<stock_len> stock_t;
typedef ap_uint<qty_len> qty_t;
typedef ap_uint<price_len> price_t;
struct user_obj
{
    int stock;
    int price;
    int qty;
};

// Top function
void riskcontrol(hls::stream<user_t> &inputdata);