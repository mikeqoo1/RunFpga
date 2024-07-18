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

struct Order
{
    int stockno;
    int price;
    int qty;
    int bs;
};

struct Limit
{
    int stockno;
    int qty;
};

Order sw_orderlist[10];
Limit sw_limitlist[10];

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

int QueryOrder(int64_t *list)
{
    int i = 0;
    MYSQL_RES *res;
    MYSQL_ROW row;
    stringstream sql_query;
    sql_query.str(""); // 字串流清零，將流中的資料全部清除
    sql_query.clear();
    sql_query << "SELECT * FROM `Order`";
    if (mysql_real_query(conndb, sql_query.str().c_str(), sql_query.str().length()))
    {
        cout << "QueryOrder ERROR: " << string(mysql_error(conndb)) << endl;
        cout << "ERROR query:" << sql_query.str() << endl;
        return -1;
    }

    res = mysql_use_result(conndb);
    Order order_obj;
    string key;
    while ((row = mysql_fetch_row(res)) != NULL)
    {

        order_obj.stockno = atoi(row[0]);
        order_obj.price = atoi(row[1]);
        order_obj.qty = atoi(row[2]);
        order_obj.bs = atoi(row[3]);
        std::stringstream ss;
        ss.str("");
        ss.clear();
        ss << order_obj.stockno << order_obj.price << order_obj.qty << order_obj.bs;
        cout << "Order data:[" << ss.str() << "]" << endl;
        list[i] = atoll(ss.str().c_str());
        sw_orderlist[i] = order_obj;
        i++;
    }
    mysql_free_result(res);
    return 0;
}

int QueryLimit(int64_t *list)
{
    int i = 0;
    MYSQL_RES *res;
    MYSQL_ROW row;
    stringstream sql_query;
    sql_query.str(""); // 字串流清零，將流中的資料全部清除
    sql_query.clear();
    sql_query << "SELECT * FROM `Limit`";
    if (mysql_real_query(conndb, sql_query.str().c_str(), sql_query.str().length()))
    {
        cout << "QueryLimit ERROR: " << string(mysql_error(conndb)) << endl;
        cout << "ERROR query:" << sql_query.str() << endl;
        return -1;
    }

    res = mysql_use_result(conndb);
    Limit limit_obj;
    while ((row = mysql_fetch_row(res)) != NULL)
    {

        limit_obj.stockno = atoi(row[0]);
        limit_obj.qty = atoi(row[1]);
        std::stringstream ss;
        ss.str("");
        ss.clear();
        ss << limit_obj.stockno << limit_obj.qty;
        cout << "Limit data:[" << ss.str() << "]" << endl;
        list[i] = atoll(ss.str().c_str());
        sw_limitlist[i] = limit_obj;
        i++;
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
    int64_t orderlist[100];
    int64_t limitlist[100];
    int status;
    status = ConnectDB();
    if (status != 0)
    {
        cout << "status=" << status << endl;
        exit(1);
    }

    status = QueryOrder(orderlist);
    if (status != 0)
    {
        cout << "status=" << status << endl;
        exit(1);
    }

    status = QueryLimit(limitlist);
    if (status != 0)
    {
        cout << "status=" << status << endl;
        exit(1);
    }

    //------以上從資料庫拿完資料------//

    int sw_ans[50];
    for (int i = 0; i <= 50; i++)
    {
        if (sw_orderlist[i].bs == 1)
        {
            if (5000000 < sw_orderlist[i].qty * sw_orderlist[i].price)
            {
                sw_ans[i] = 1;
            }
            else
            {
                sw_ans[i] = 0;
            }
        }
        else if (sw_orderlist[i].bs == 2)
        {
            if (sw_limitlist[i].stockno == sw_orderlist[i].stockno)
            {
                if (sw_limitlist[i].qty < sw_orderlist[i].qty)
                {
                    sw_ans[i] = 1;
                }
                else
                {
                    sw_ans[i] = 0;
                }
            }
        }
        else
        {
            sw_ans[i] = 0;
        }
        cout << "ANS:[" << sw_ans[i] << "]" << endl;
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

    auto krnl = xrt::kernel(device, uuid, "riskcontrol");

    std::cout << "Allocate Buffer in Global Memory\n";
    auto device_order_data = xrt::bo(device, vector_size_bytes, krnl.group_id(0));
    auto device_result = xrt::bo(device, vector_size_bytes, krnl.group_id(1));

    device_order_data.write(orderlist);

    // 把硬體結果接回來本地
    auto result = device_result.map<int *>();

    // Synchronize buffer content with device side
    std::cout << "synchronize input buffer data to device global memory\n";

    device_order_data.sync(XCL_BO_SYNC_BO_TO_DEVICE);

    std::cout << "Execution of the kernel\n";
    auto run = krnl(device_order_data, device_result);
    run.wait();

    // Get the output;
    std::cout << "Get the output data from the device" << std::endl;
    device_result.sync(XCL_BO_SYNC_BO_FROM_DEVICE);

    // memcmp 是用來判斷兩段記憶體區塊內容是否相同的函式
    int ret = std::memcmp(result, sw_ans, DATA_SIZE);
    if (ret > 0)
    {
        std::cout << "device_result is greater than bufReference" << std::endl;
    }
    else if (ret < 0)
    {
        std::cout << "device_result is less than bufReference" << std::endl;
    }
    else
    {
        std::cout << "結果一樣?" << std::endl;
    }
    std::cout << "bufReference=" << &sw_ans << std::endl;
    std::cout << "device_result=" << &device_result << std::endl;

    std::cout << "TEST PASSED\n";
    return 0;
}
