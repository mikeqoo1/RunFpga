#include "parsetmpmsg.h"

// 開始處理: UpG04MtDpG04Mt202401091000026640143014T1234  400000002200002940001      0qNN
// `bhno` = 'U', 0
// `mseqno` = 'pG04Mt', 1~6
// `msgtype` = 'D', 7
// `seqno` = 'pG04Mt', 8~13
// `tdate` = '20240109', 14~21
// `ttime` = '100002664', 22~30
// `accno` = '014301', 31~36
// `accck` = '4', 37
// `mtype` = 'T', 38
// `symbol` = '1234  ', 39~44
// `iflag` = '4', 45
// `otype` = '0', 46
// `ecode` = '0', 47
// `orderqty` = '000002', 48~53
// `ordtype` = '2', 54
// `timeinforce` = '0', 55
// `price` = '000294000', 56~64
// `side` = '1', 65
// `sales` = '   ', 66~68
// `keyin` = '   ', 69~71
// `fcode` = '0', 72
// `origin` = 'q', 73
// `dtrade` = 'N', 74
// `siplan` = 'N', 75
// `reserved` = '      ' 76~81


// 1. 展開循環：
// 循環展開可以通過允許並行執行多個迭代來提高吞吐量
// 然而，這可能會導致資源使用增加，因此需要在資源使用和吞吐量之間找到平衡
// 可以使用 #pragma HLS UNROLL 這樣的指示來請求循環展開

// 2. 流水線化：
// 這個指示表示每個迭代的循環可以在一個時鐘周期內執行 可以嘗試調整 II (啟動間隔) 的值，以找到最適合你的設計的值

// 用位移的方式比較快
void parseOrder(char *input, order_data &output)
{
    output.bhno = (ap_uint<8>)input[0];

    output.mseqno = 0;
    for (int i = 1; i <= 6; ++i)
    {
        output.mseqno = (output.mseqno << 8) | input[i];
    }

    output.msgtype = (ap_uint<8>)input[7];

    output.seqno = 0;
    for (int i = 8; i <= 13; ++i)
    {
        output.seqno = (output.seqno << 8) | input[i];
    }

    output.tdate = 0;
    for (int i = 14; i <= 21; ++i)
    {
        output.tdate = (output.tdate << 8) | (input[i]);
    }

    output.ttime = 0;
    for (int i = 22; i <= 30; ++i)
    {
        output.ttime = (output.ttime << 8) | (input[i]);
    }

    output.accno = 0;
    for (int i = 31; i <= 36; ++i)
    {
        output.accno = (output.accno << 8) | (input[i]);
    }

    output.accck = (ap_uint<8>)input[37];

    output.mtype = (ap_uint<8>)input[38];

    output.symbol = 0;
    for (int i = 39; i <= 44; ++i)
    {
        output.symbol = (output.symbol << 8) | input[i];
    }

    output.iflag = (ap_uint<8>)input[45];
    output.otype = (ap_uint<8>)input[46];
    output.ecode = (ap_uint<8>)input[47];

    output.orderqty = 0;
    for (int i = 48; i <= 53; ++i)
    {
        output.orderqty = (output.orderqty << 8) | (input[i]);
    }

    output.ordtype = (ap_uint<8>)input[54];
    output.timeinforce = (ap_uint<8>)input[55];

    output.price = 0;
    for (int i = 56; i <= 64; ++i)
    {
        output.price = (output.price << 8) | (input[i]);
    }

    output.side = (ap_uint<8>)input[65];

    output.sales = 0;
    for (int i = 66; i <= 68; ++i)
    {
        output.sales = (output.sales << 8) | (input[i]);
    }

    output.keyin = 0;
    for (int i = 69; i <= 71; ++i)
    {
        output.keyin = (output.keyin << 8) | (input[i]);
    }

    output.fcode = (ap_uint<8>)input[72];
    output.origin = (ap_uint<8>)input[73];
    output.dtrade = (ap_uint<8>)input[74];
    output.siplan = (ap_uint<8>)input[75];

    output.reserved = 0;
    for (int i = 76; i <= 81; ++i)
    {
        output.reserved = (output.reserved << 8) | (input[i]);
    }
}
