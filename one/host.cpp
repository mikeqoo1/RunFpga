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

#include "cmdlineparser.h"

// FPGA 相關
#include "experimental/xrt_bo.h"
#include "experimental/xrt_device.h"
#include "experimental/xrt_kernel.h"

using namespace std;

#define DATA_SIZE 256

MYSQL *conndb;
struct Account
{
    int account_no;
    int amt;
};

struct Stock
{
    int account_no;
    int stockno;
    int qty;
};

int ConnectDB();
int QueryAccount(map<int, Account> *accmap);
int QueryStock(map<int, Stock> *stockmap);

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

int QueryStock(map<int, Stock> *stockmap)
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
        s.stockno = atoi(row[1]);
        s.qty = atoi(row[2]);
        std::stringstream ss;
        ss.str("");
        ss.clear();
        ss << s.account_no << s.stockno;
        key = ss.str();
        stockmap->insert(std::pair<int, Stock>(atoi(key.c_str()), s));
    }
    mysql_free_result(res);
    return 0;
}

int SearchAmt(std::vector<int> account_vector, int account)
{
    int account_to_find = account;
    auto it = std::find(account_vector.begin(), account_vector.end(), account_to_find);
    if (it != account_vector.end())
    {
        // 帳號的index 一定是偶數
        int index = std::distance(account_vector.begin(), it);
        if (index % 2 == 0)
        {
            if (index + 1 < int(account_vector.size()))
            {
                int amt = account_vector[index + 1]; // 找帳號對應的度
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

int main(int argc, char **argv)
{
    map<int, Account> accmap;
    map<int, Stock> stockmap;
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
    std::vector<int> account_vector;

    for (const auto &pair : vec)
    {
        account_vector.push_back(pair.second.account_no); // 先加帳號
        account_vector.push_back(pair.second.amt);        // 再加額度
    }

    std::vector<std::pair<int, Stock>> vec2(stockmap.begin(), stockmap.end());
    std::vector<int> stock_vector;
    for (const auto &pair2 : vec2)
    {
        stock_vector.push_back(pair2.second.account_no);
        stock_vector.push_back(pair2.second.stockno);
        stock_vector.push_back(pair2.second.qty);
    }

    std::vector<int> result_sw;
    std::vector<int> result_hw;

    std::cout << "帳號跟額度:" << std::endl;
    for (size_t i = 0; i < account_vector.size(); ++i)
    {
        std::cout << account_vector[i] << std::endl;
        // if ((i + 1) % 10 == 0)
        //     std::cout << std::endl;
    }
    SearchAmt(account_vector, 666);

    std::cout << "庫存:" << std::endl;
    for (size_t i = 0; i < stock_vector.size(); ++i)
    {
        std::cout << stock_vector[i] << std::endl;
        // if ((i + 1) % 10 == 0)
        //     std::cout << std::endl;
    }

    // Command Line Parser
    sda::utils::CmdLineParser parser;

    // Switches
    //**************//"<Full Arg>",  "<Short Arg>", "<Description>", "<Default>"
    parser.addSwitch("--xclbin_file", "-x", "input binary file string", "");
    parser.addSwitch("--device_id", "-d", "device index", "0");
    parser.parse(argc, argv);

    // Read settings
    std::string binaryFile = parser.value("xclbin_file");
    int device_index = stoi(parser.value("device_id"));

    if (argc < 3)
    {
        parser.printHelp();
        return EXIT_FAILURE;
    }

    std::cout << "Open the device" << device_index << std::endl;
    auto device = xrt::device(device_index);
    std::cout << "Load the xclbin " << binaryFile << std::endl;
    auto uuid = device.load_xclbin(binaryFile);

    size_t vector_size_bytes = sizeof(int) * DATA_SIZE;

    // auto krnl = xrt::kernel(device, uuid, "krnl_vadd");
    auto krnl = xrt::kernel(device, uuid, "riskcontrol");

    std::cout << "Allocate Buffer in Global Memory\n";
    auto device_account_vector = xrt::bo(device, vector_size_bytes, krnl.group_id(0));
    auto device_stock_vector = xrt::bo(device, vector_size_bytes, krnl.group_id(1));
    auto result = xrt::bo(device, vector_size_bytes, krnl.group_id(2));

    // copy 需要研究一下
    // device_account_vector.copy(account_vector);
    // device_stock_vector.copy(stock_vector);

    // Write 沒辦法把 vector 放進去 需要轉換成 array 的型態
    int *a = &account_vector[0];
    int *b = &stock_vector[0];
    device_account_vector.write(a);
    device_stock_vector.write(b);

    // 把硬體結果接回來本地
    auto result_device = result.map<int *>();

    // Synchronize buffer content with device side
    std::cout << "synchronize input buffer data to device global memory\n";

    device_account_vector.sync(XCL_BO_SYNC_BO_TO_DEVICE);
    device_stock_vector.sync(XCL_BO_SYNC_BO_TO_DEVICE);

    std::cout << "Execution of the kernel\n";
    auto run = krnl(device_account_vector, device_stock_vector, result, DATA_SIZE);
    run.wait();

    // Get the output;
    std::cout << "Get the output data from the device" << std::endl;
    result.sync(XCL_BO_SYNC_BO_FROM_DEVICE);

    // memcmp 是用來判斷兩段記憶體區塊內容是否相同的函式
    //if (std::memcmp(result_device, 預計的內容, DATA_SIZE))
    //    throw std::runtime_error("Value read back does not match reference");
    std::cout << "result_device=" << result_device << std::endl;

    std::cout << "TEST PASSED\n";
    return 0;
}
