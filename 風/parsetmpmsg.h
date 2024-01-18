#include <ap_int.h>

// 固定長度版本
struct order_data
{
    // 一個字節 (byte) 都是 8 bits 所以都要寬度 * 8
    ap_uint<8> bhno;        // 文數字長度 1
    ap_uint<48> mseqno;     // 文數字長度 6
    ap_uint<8> msgtype;     // 文數字長度 1
    ap_uint<48> seqno;      // 文數字長度 6
    ap_uint<64> tdate;      // 數字長度 8
    ap_uint<72> ttime;      // 文數字長度 9
    ap_uint<48> accno;      // 數字長度 6
    ap_uint<8> accck;       // 數字長度 1
    ap_uint<8> mtype;       // 文數字長度 1
    ap_uint<48> symbol;     // 文數字長度 6
    ap_uint<8> iflag;       // 文數字長度 1
    ap_uint<8> otype;       // 文數字長度 1
    ap_uint<8> ecode;       // 文數字長度 1
    ap_uint<48> orderqty;   // 數字長度 6
    ap_uint<8> ordtype;     // 數字長度 1
    ap_uint<8> timeinforce; // 文數字長度 1
    ap_uint<72> price;      // 數字長度 9
    ap_uint<8> side;        // 數字長度 1
    ap_uint<24> sales;      // 文數字長度 3
    ap_uint<24> keyin;      // 文數字長度 3
    ap_uint<8> fcode;       // 文數字長度 1
    ap_uint<8> origin;      // 文數字長度 1
    ap_uint<8> dtrade;      // 文數字長度 1
    ap_uint<8> siplan;      // 文數字長度 1
    ap_uint<48> reserved;   // 文數字長度 6
};

void parseOrder(char *input, order_data &output);