#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include "parsetmpmsg.h"
// (mseqno>> 40) & 0xFF：將 mseqno 右移 40 位，然後使用位運算 & 0xFF 保留最低的 8 位，得到第一個字節的數值。
// (mseqno>> 32) & 0xFF：類似地，獲取第二個字節的數值。
// (mseqno>> 24) & 0xFF、(mseqno >> 16) & 0xFF、(mseqno >> 8) & 0xFF、mseqno & 0xFF：獲取第三、四、五、六個字節的數值。
// 這樣就得到了一個包含六個字符的字符串，每個字符都代表 mseqno 中對應位置的字節。這個函數可以用於將數值轉換為字符表示，以便在輸出時更容易理解。
// std::string convertMseqnoToString(ap_uint<48> mseqno) {
//     std::string result;
//     result.push_back(static_cast<char>((mseqno >> 40) & 0xFF));
//     result.push_back(static_cast<char>((mseqno >> 32) & 0xFF));
//     result.push_back(static_cast<char>((mseqno >> 24) & 0xFF));
//     result.push_back(static_cast<char>((mseqno >> 16) & 0xFF));
//     result.push_back(static_cast<char>((mseqno >> 8) & 0xFF));
//     result.push_back(static_cast<char>(mseqno & 0xFF));
//     return result;
// }

// 轉換函數
template <std::size_t BitSize>
std::string convertToString(ap_uint<BitSize> data)
{
    static_assert(BitSize % 8 == 0, "BitSize must be a multiple of 8");

    std::string result;
    for (std::size_t i = BitSize; i > 0; i -= 8)
    {
        result.push_back(static_cast<char>((data >> (i - 8)) & 0xFF));
    }
    return result;
}

int main()
{
    char *inputData = "AZE7J06DZE7J06202401160830000191393504T00937B640000005200001753002       ZNN";
    order_data result;
    parseOrder(inputData, result);
    std::cout << "Parsed Order:" << std::endl;
    std::cout << "bhno: " << result.bhno.to_char() << std::endl;
    std::cout << "mseqno: " << convertToString<48>(result.mseqno) << std::endl;
    std::cout << "msgtype: " << result.msgtype << std::endl;
    std::cout << "seqno: " << result.seqno << std::endl;
    std::cout << "tdate: " << result.tdate << std::endl;
    std::cout << "ttime: " << result.ttime << std::endl;
    std::cout << "accno: " << result.accno << std::endl;
    std::cout << "accck: " << result.accck << std::endl;
    std::cout << "mtype: " << result.mtype << std::endl;
    std::cout << "symbol: " << result.symbol << std::endl;
    std::cout << "iflag: " << result.iflag << std::endl;
    std::cout << "otype: " << result.otype << std::endl;
    std::cout << "ecode: " << result.ecode << std::endl;
    std::cout << "orderqty: " << result.orderqty << std::endl;
    std::cout << "ordtype: " << result.ordtype << std::endl;
    std::cout << "timeinforce: " << result.timeinforce << std::endl;
    std::cout << "price: " << result.price << std::endl;
    std::cout << "side: " << result.side << std::endl;
    std::cout << "sales: " << result.sales << std::endl;
    std::cout << "keyin: " << result.keyin << std::endl;
    std::cout << "fcode: " << result.fcode << std::endl;
    std::cout << "origin: " << result.origin << std::endl;
    std::cout << "dtrade: " << result.dtrade << std::endl;
    std::cout << "siplan: " << result.siplan << std::endl;
    std::cout << "reserved: " << result.reserved << std::endl;
    return 0;
}
