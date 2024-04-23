#include <iostream>
#include <string>
#include <cstring>
#include <sstream>
#include <map>
#include <vector>
#include <algorithm>
#include <numeric> // For std::transform_reduce
#include <iomanip> // For std::setw
#include <mysql.h>

// FPGA 相關
// #include "xcl2.hpp"
// #include <xrt/xrt_device.h>
// #include <experimental/xrt_xclbin.h>
// #include <xrt/xrt_bo.h>
// #include <xrt/xrt_kernel.h>
// #include <experimental/xrt_ip.h>

using namespace std;

template <typename T>
class aligned_allocator
{
public:
    using value_type = T;
    using pointer = T *;
    using const_pointer = const T *;
    using reference = T &;
    using const_reference = const T &;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;

    aligned_allocator() noexcept = default;
    ~aligned_allocator() noexcept = default;

    template <typename U>
    struct rebind
    {
        using other = aligned_allocator<U>;
    };

    pointer allocate(size_type n)
    {
        if (auto p = static_cast<pointer>(aligned_alloc(alignof(T), n * sizeof(T))))
            return p;
        throw std::bad_alloc();
    }

    void deallocate(pointer p, size_type) noexcept
    {
        std::free(p);
    }
};

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

int SearchAmt(std::vector<int, aligned_allocator<int>> source_a, int account)
{
    int account_to_find = account;
    auto it = std::find(source_a.begin(), source_a.end(), account_to_find);
    if (it != source_a.end())
    {
        // 帳號的index 一定是偶數
        int index = std::distance(source_a.begin(), it);
        if (index % 2 == 0)
        {
            if (index + 1 < source_a.size())
            {
                int amt = source_a[index + 1]; // 找帳號對應的度
                std::cout << "帳號 " << account_to_find << " 的額度是：" << amt << std::endl;
                return amt;
            }
        }
        else
        {
            std::cout << "未找到帳號 " << account_to_find << " 的額度" << std::endl;
        }
    }
    else
    {
        std::cout << "無此帳號 " << account_to_find << std::endl;
    }
    return -1;
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
    // Convert map to vector
    std::vector<std::pair<int, Account>> vec(accmap.begin(), accmap.end());

    // Extract 'amt' from each Account object and store in vector
    std::vector<int, aligned_allocator<int>> source_a;
    source_a.reserve(vec.size() * 2); // Reserve space for both 'amt' and 'account_no'

    for (const auto &pair : vec)
    {
        source_a.push_back(pair.second.account_no); // 先加帳號
        source_a.push_back(pair.second.amt);        // 再加額度
    }

    std::cout << "帳號跟額度:" << std::endl;
    for (size_t i = 0; i < source_a.size(); ++i)
    {
        std::cout << std::setw(5) << source_a[i] << " ";
        if ((i + 1) % 10 == 0)
            std::cout << std::endl;
    }
    std::cout << std::endl;

    SearchAmt(source_a, 666);
}