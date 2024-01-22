#include <benchmark/benchmark.h>
#include <iostream>
#include <sstream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 分割方法
void split0(std::vector<std::string> &vec, std::string str, char delim)
{
    std::stringstream ss(str);
    std::string word;
    while (std::getline(ss, word, delim))
    {
        vec.push_back(word);
    }
}

void split1(std::vector<std::string> &vec, std::string str, char delim)
{
    size_t pos = str.find(delim);
    while (pos != std::string::npos)
    {
        vec.emplace_back(str, 0, pos);
        std::string new_s = str.substr(pos + 1);
        str.swap(new_s);
        pos = str.find(delim);
    }

    if (str.size() > 0)
    {
        vec.push_back(str);
    }
}

void split2(std::vector<std::string> &vec, std::string str, char delim)
{
    size_t start = str.find_first_not_of(delim);
    size_t end;

    while (start != std::string::npos)
    {
        end = str.find_first_of(delim, start + 1);
        if (end == std::string::npos)
        {
            // vec.push_back(str.substr(start));
            vec.emplace_back(str, start);
            break;
        }
        else
        {
            // vec.push_back(str.substr(start, end - start));
            vec.emplace_back(str, start, end - start);
        }
        start = str.find_first_not_of(delim, end + 1);
    }
}

void split3(std::vector<std::string> &vec, std::string str, char delim)
{
    int start = 0;
    while (start < str.size())
    {
        size_t end = str.find(delim, start);
        if (end == std::string::npos)
        {
            vec.emplace_back(str, start);
            break;
        }
        vec.emplace_back(str, start, end - start);
        start = end + 1;
    }
}

void strtok_split(std::vector<std::string> &vec, std::string str, char delim)
{
    const int MAX_BUF_SIZE = 1024;
    char *save = NULL;
    char buf[MAX_BUF_SIZE];
    char del[2] = "";
    del[0] = delim;
    int ret = snprintf(buf, sizeof(buf), "%s", str.c_str());
    if (ret < 0)
    {
        return;
    }
    char *token = strtok_r(buf, del, &save);
    while (token != NULL)
    {
        vec.emplace_back(token);
        token = strtok_r(NULL, del, &save);
    }
}

void tmpmsg(std::string str)
{
    std::string bhno = str.substr(0, 1);
    std::string mseqno = str.substr(1, 6);
    std::string msgtype = str.substr(7, 1);
    std::string seqno = str.substr(8, 6);
    std::string tdate = str.substr(14, 8);
    std::string ttime = str.substr(22, 9);
    std::string accno = str.substr(31, 6);
    std::string accck = str.substr(37, 1);
    std::string mtype = str.substr(38, 1);
    std::string symbol = str.substr(39, 6);
    std::string iflag = str.substr(45, 1);
    std::string otype = str.substr(46, 1);
    std::string ecode = str.substr(47, 1);
    std::string orderqty = str.substr(48, 6);
    std::string ordtype = str.substr(54, 1);
    std::string timeinforce = str.substr(55, 1);
    std::string price = str.substr(56, 5) + "." + str.substr(61, 4);
    std::string side = str.substr(65, 1);
    std::string sales = str.substr(66, 3);
    std::string keyin = str.substr(69, 3);
    std::string fcode = str.substr(72, 1);
    std::string origin = str.substr(73, 1);
    std::string dtrade = str.substr(74, 1);
    std::string siplan = str.substr(75, 1);
    std::string reserved = str.substr(76, 6);
}

// 测试字符串
const std::string global_str = "35=999`50008=1`1=1300018`37=10001`38=1`40=2`44=15`50=8450`54=B`55=6016`56=O`59=0`60=20240112134012617`10000=1`10001=0`50001=06`9487=Y`9488=1`50002=110101`50003= `50004=N`50005=N`50009=0`60001=N`60002=N`60003=N`60004=`60005=N`";
const std::string tmp_global_str = "AZE7J06DZE7J06202401160830000191393504T00937B640000005200001753002       ZNN      ";

static void BM_split0(benchmark::State &state)
{
    for (auto _ : state)
    {
        std::vector<std::string> vec;
        split0(vec, global_str, '`');
    }
}

static void BM_split1(benchmark::State &state)
{
    for (auto _ : state)
    {
        std::vector<std::string> vec;
        split1(vec, global_str, '`');
    }
}

static void BM_split2(benchmark::State &state)
{
    for (auto _ : state)
    {
        std::vector<std::string> vec;
        split2(vec, global_str, '`');
    }
}
static void BM_split3(benchmark::State &state)
{
    for (auto _ : state)
    {
        std::vector<std::string> vec;
        split3(vec, global_str, '`');
    }
}

static void BM_split4(benchmark::State &state)
{
    for (auto _ : state)
    {
        std::vector<std::string> vec;
        strtok_split(vec, global_str, '`');
    }
}

static void BM_tmpmsg(benchmark::State &state)
{
    for (auto _ : state)
    {
        tmpmsg(tmp_global_str);
    }
}


BENCHMARK(BM_split0);
BENCHMARK(BM_split1);
BENCHMARK(BM_split2);
BENCHMARK(BM_split3);
BENCHMARK(BM_split4);
BENCHMARK(BM_tmpmsg);
BENCHMARK_MAIN();