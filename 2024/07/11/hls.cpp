#include "hls.hpp"

void riskcontrol(hls::stream<order_t> &inputdata, hls::stream<db_stock_t> &db_stock_qty, hls::stream<result_t> &qty_out, hls::stream<result_t> &price_out)
{
    hls::stream<stock_t> stockid;
    hls::stream<qty_t> qty;
    hls::stream<price_t> price;
    hls::stream<bs_t> bs;
    hls::stream<int> use_amt;

    hls::stream<stock_t> db_stockid;
    hls::stream<qty_t> db_qty;
#pragma HLS INTERFACE m_axi port = inputdata bundle = gmem0 depth = 128
#pragma HLS INTERFACE m_axi port = db_stock_qty bundle = gmem1 depth = 128
#pragma HLS dataflow
    getinventory(db_stock_qty, db_stockid, db_qty);
    dataassign(inputdata, stockid, price, qty, bs, use_amt); // 處理資料
    bs_t bs_code = bs.read();
    // stock_t stock = stockid.read();
    if (bs_code == 1)
    {
        checkPrice(price, use_amt, price_out, qty_out); // 檢查額度
    }
    else
    {
        checkQty(qty, stockid, db_stockid, db_qty, qty_out, price_out); // 檢查庫存
    }
}

void dataassign(hls::stream<order_t> &in, hls::stream<stock_t> &stock, hls::stream<price_t> &price, hls::stream<qty_t> &qty, hls::stream<bs_t> &bs, hls::stream<int> &use_amt)
{
    int money;

    for (int i = 0; i < num_users; i++)
    {
        order_t order = in.read();
        // printf("資料: [%d] \n", order);

        stock_t tempstock = order(stock_len + price_len + qty_len + bs_len - 1, price_len + qty_len + bs_len);
        // printf("stock: [%d] \n", tempstock);

        price_t tempprice = order(price_len + qty_len + bs_len - 1, price_len);
        // printf("price: [%d] \n", tempprice);

        qty_t tempqty = order(qty_len + bs_len - 1, qty_len);
        // printf("qty: [%d] \n", tempqty);

        bs_t tempbs = order(bs_len - 1, 0);
        // printf("bs: [%d] \n", tempbs);

        money = tempqty * tempprice;
        printf("money: [%d] \n", money);
        if (i == 0)
            printf("dataassign i=%d 資料: [%d] stock: [%d] price: [%d] qty: [%d] bs: [%d]\n", i, order, tempstock, tempprice, tempqty, tempbs);

        stock.write(tempstock);
        price.write(tempprice);
        qty.write(tempqty);
        bs.write(tempbs);
        use_amt.write(money);
    }
}

void getinventory(hls::stream<db_stock_t> &db, hls::stream<stock_t> &db_stock, hls::stream<qty_t> &db_qty)
{
    for (int i = 0; i < num_users; i++)
    {
        db_stock_t data = db.read();
        //printf("資料庫資料: [%d] \n", data);

        stock_t tempstock = data(stock_len + qty_len - 1, qty_len);
        // printf("資料庫stock: [%d] \n", tempstock);

        qty_t tempqty = data(qty_len - 1, 0);
        // printf("資料庫庫存: [%d] \n", tempqty);
        if (i == 0)
            printf("getinventory i=%d 資料: [%d] stock: [%d] qty: [%d]\n", i, data, tempstock, tempqty);
        db_stock.write(tempstock);
        db_qty.write(tempqty);
    }
}

void checkQty(hls::stream<qty_t> &qty, hls::stream<stock_t> &stock,
              hls::stream<stock_t> &db_stockid, hls::stream<qty_t> &db_qty,
              hls::stream<result_t> &check_qty_out, hls::stream<result_t> &check_price_out)
{
    for (int i = 0; i < num_users; i++)
    {
        qty_t q = qty.read();
        //printf("qty: %d\n", q);
        stock_t s = stock.read();
        //printf("stock: %d\n", s);
        stock_t dbstockid = db_stockid.read();
        //printf("dbstockid: %d\n", dbstockid);
        qty_t dbqty = db_qty.read();
        //printf("dbqty: %d\n", dbqty);
        result_t result;
        result = 0;
        if (s == dbstockid)
        {
            if (q > dbqty)
            {
                result = 1;
            }
            else
            {
                result = 0;
            }
            // printf("qty: %d, 結果: %d \n", q, result);
        }
        check_qty_out.write(result);
        check_price_out.write(0);
    }
}

void checkPrice(hls::stream<price_t> &price, hls::stream<int> &use_amt,
                hls::stream<result_t> &check_price_out, hls::stream<result_t> &check_qty_out)
{
    for (int i = 0; i < num_users; i++)
    {
        price_t p = price.read();
        //printf("price: %d\n", p);
        int u = use_amt.read();
        printf("use_amt: %d \n", u);
        result_t result;
        if (u > limit_amt)
        {
            result = 1;
        }
        else
        {
            result = 0;
        }
        // printf("price: %d, qty: %d amt: %d\n", p, q, amt);
        check_price_out.write(result);
        check_qty_out.write(0);
    }
}