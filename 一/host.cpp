#include <iostream>
#include <string>
#include <cstring>
#include <sstream>
#include <map>
#include <mysql.h>

// FPGA 相關
#include "xcl2.hpp"
#include <xrt/xrt_device.h>
#include <experimental/xrt_xclbin.h>
#include <xrt/xrt_bo.h>
#include <xrt/xrt_kernel.h>
#include <experimental/xrt_ip.h>

using namespace std;
MYSQL *conndb;
struct Account
{
    int account_no;
    int amt;
};

struct Stock
{
    int account_no;
    char stockno[6];
    int qty;
};

int ConnectDB();
int QueryAccount(map<string, Account> *accmap);
int QueryStock(map<string, Stock> *stockmap);

int ConnectDB()
{
    conndb = mysql_init(NULL);
    mysql_options(conndb, MYSQL_OPT_NONBLOCK, 0);
    if (!mysql_real_connect(conndb, "192.168.199.235", "crcft", "Aa1234", "fpgatest", 3306, NULL, 0))
    {
        cout << "連接資料庫錯誤" << endl;
        mysql_close(conndb);
        return -1;
    }
    return 0;
}

int QueryAccount(map<int, Account> *accmap)
{
    MYSQL_RES *res;
    MYSQL_ROW row;
    stringstream sql_query;
    sql_query.str(""); // 字串流清零，將流中的資料全部清除
    sql_query.clear();
    sql_query << "SELECT * FROM `TestAccount`";
    if (mysql_real_query(conndb, sql_query.str().c_str(), sql_query.str().length()))
    {
        cout << "QueryAccount ERROR: " << string(mysql_error(conndb)) << endl;
        cout << "ERROR query:" << sql_query.str() << endl;
        return -1;
    }

    res = mysql_use_result(conndb);
    Account a;

    while ((row = mysql_fetch_row(res)) != NULL)
    {
        a.account_no = atoi(row[0]);
        a.amt = atoi(row[1]);
        accmap->insert(std::pair<int, Account>(a.account_no, a));
    }
    mysql_free_result(res);
    return 0;
}

int QueryStock(map<string, Stock> *stockmap)
{
    MYSQL_RES *res;
    MYSQL_ROW row;
    stringstream sql_query;
    sql_query.str(""); // 字串流清零，將流中的資料全部清除
    sql_query.clear();
    sql_query << "SELECT * FROM `TestStock`";
    if (mysql_real_query(conndb, sql_query.str().c_str(), sql_query.str().length()))
    {
        cout << "QueryStock ERROR: " << string(mysql_error(conndb)) << endl;
        cout << "ERROR query:" << sql_query.str() << endl;
        return -1;
    }

    res = mysql_use_result(conndb);
    Stock s;
    string key;
    while ((row = mysql_fetch_row(res)) != NULL)
    {
        s.account_no = atoi(row[0]);
        strcpy(s.stockno, row[1]);
        s.qty = atoi(row[2]);
        std::stringstream ss;
        ss.str("");
        ss.clear();
        ss << s.account_no << s.stockno;
        key = ss.str();
        stockmap->insert(std::pair<string, Stock>(key, s));
    }
    mysql_free_result(res);
    return 0;
}

int main()
{
    map<int, Account> accmap;
    map<string, Stock> stockmap;
    int status;
    status = ConnectDB();
    if (status != 0)
    {
        cout << "status=" << status << endl;
        exit(1);
    }

    status = QueryAccount(&accmap);
    if (status != 0)
    {
        cout << "status=" << status << endl;
        exit(1);
    }
    else
    {
        for (auto it = accmap.cbegin(); it != accmap.cend(); ++it)
        {
            cout << "key=[" << it->first << "]帳號:" << it->second.account_no << " 額度:" << it->second.amt << endl;
        }
    }

    status = QueryStock(&stockmap);
    if (status != 0)
    {
        cout << "status=" << status << endl;
        exit(1);
    }
    else
    {
        for (auto it = stockmap.cbegin(); it != stockmap.cend(); ++it)
        {
            cout << "key=[" << it->first << "]帳號:" << it->second.account_no << " 股票:" << it->second.stockno << " 庫存:" << it->second.qty << endl;
        }
    }

    //------以上從資料庫拿完資料------//

    // Vitis hls 不支援 C++ Map 所以這邊要做一點轉換我把 C++ Map 轉成 Vector
}