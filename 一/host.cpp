#include <iostream.h>
#include <mysql.h>
using namespace std;

MYSQL *conndb;

struct Account {
    int account_no;
    int amt;
};

struct Stock {
    int account_no;
     amt;
};

map<string, Account> accmap;



int ConnectDB();
int QueryAccount(map<string, Account> accmap);
int QueryStock(map<string, Stock> *stocks);

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

int main()
{

}