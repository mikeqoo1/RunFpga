# HLS 函數

頂層函數綜合後就會變 RTL 設計的頂層模組, 綜合後, 設計中每個函數都有自己的綜合報告和 HDL 文件(Verilog 和 VHDL)

重要: Top Function 不得用靜態函數

## 內聯函數 (內嵌函式)

在軟體的定義: [微軟連結](https://learn.microsoft.com/zh-tw/cpp/cpp/inline-functions-cpp?view=msvc-170)

```txt
inline 關鍵字會指示編譯器針對函式呼叫的每個執行個體，取代函式定義內的程式碼。

使用內嵌函式可以讓程式更快速，因為它們不需要與函式呼叫相關聯的負荷。 編譯器可以透過無法供一般函式使用的方式，將擴充的函式優化。

內嵌程式碼替代會在編譯器的任意時間進行。 例如，如果建置函式的位址太大而無法內嵌，則編譯器不會內嵌函式。

類別宣告主體中定義的函式隱含為內嵌函式。

簡單說就是加速就對了
```

在 HLS 裡面: 
下面範例會呼叫 foo() 3 次 但是加上這段 #pragma HLS ALLOCATION instances=foo limit=1 function
把函數 foo_sub 内聯並使用 ALLOCATION 指令只使用 foo 函數的 1 個實例, 這要生成的設計只會有 1 個 foo 函數的實例
面積就會是優化前的 3 分之 1, 使用此方法可以減少更多資源

```c++
foo_sub (p, q) {
    int q1 = q + 10;
    foo(p1,q); // foo_3 呼叫 3 次
}
void foo_top { a, b, c, d} {
    #pragma HLS ALLOCATION instances=foo limit=1 function
    ...
    foo(a,b); // foo_1 呼叫 1 次
    foo(a,c); // foo_2 呼叫 2 次
    foo_sub(a,d);
    ...
}
```

## 函數的流水拍打

請看常用名詞的重要指令說明

## 函數例化

函數例化是一種最佳化技巧, 不僅可以維持函數層級的面積優勢, 還可以提供另一個強大選項
在函數的特定例子上針對性局部最佳化 可以減化函數調用的控制邏輯, 也可能改進延時和吞吐量


FUNCTION_INSTANTIATE 編譯指令可簡化周圍控制結構, 並且產生最優化的, 更小的函數模組

```c++
char func(char inval, char incr) {
    #pragma HLS INLINE OFF
    #pragma HLS FUNCTION_INSTANTIATE variable=incr
    return inval + incr;
}
 
void top(char inval1, char inval2, char inval3, char *outval1, char *outval2, char *outval3)
{
    *outval1 = func(inval1,   0);
    *outval2 = func(inval2,   1);
    *outval3 = func(inval3, 100);
}
```

如果在沒有大幅度内聯或是代码修改的情况下, 在不同層級使用該函數而導致函數難以共享, 那麼函數例化可提供改進面積的最佳方法

大量小型局部最佳化的副本比大量無法共享的大型副本更有效

## 資料類型

重要 Vitis HLS 不支援 32 位元, std::complex<long double>也不支持, 在定義#define 名稱的時候 不要用其他 Lib 會用的名字怕衝突名字 (#define _TYPES_H 就盡量不要)

資料類型會影響精確度跟存儲器要求, 也會影響綜合後的 RTL 現實中的面積和性能

Vitis HLS 支持標準 C/C++ 類型進行综合, 包括精確寬度的整數類型

- (unsigned) char and (unsigned) short and (unsigned) int
- (unsigned) long and (unsigned) long long
- (unsigned) intN_t（N 是 8 16 32 64，在 stdint.h 中定義）
- float and double

AMD 是建議再一個公用的標頭檔定義所有變數的類型

AMD 的意思是這樣比較方便調整, 萬一要調整面積大小 要動到變數的時候比較方便

## 標準類型

[資料寬度](https://learn.microsoft.com/zh-tw/cpp/cpp/data-type-ranges?view=msvc-170)

```c++
標頭檔的定義
typedef char din_A; // 1 Bytes = 8 bit
typedef short din_B; // 2 Bytes = 16 bit
typedef int din_C; // 4 Bytes = 32 bit
typedef long long din_D; // 8 Bytes = 64 bit

typedef int dout_1; // 4 Bytes = 32 bit
typedef unsigned char dout_2; // 1 Bytes = 8 bit
typedef int32_t dout_3; // 4 Bytes = 32 bit
typedef int64_t dout_4; // 8 Bytes = 64 bit

void types_standard(din_A inA, din_B inB, din_C inC, din_D inD, dout_1 *out1, dout_2 *out2, dout_3 *out3, dout_4 *out4);

#include "types_standard.h"

void types_standard(din_A inA, din_B inB, din_C inC, din_D inD, dout_1 *out1, dout_2 *out2, dout_3 *out3, dout_4 *out4)
{
    // Basic arithmetic operations
    *out1 = inA * inB; 輸出是 32 bit 的資料, 乘法器是 8+16=24 bit, 會自動擴展變 32 bit 跟輸出一樣
    *out2 = inB + inA; 輸出是 8 bit 的資料, 加法器是 short 16 位裡的 8 + char 的 8 = 8 bit
    *out3 = inC / inA; 輸出是 32 bit 的資料, 會把 inA char 擴成 32 bit 再來做除法 inC 本來就 32 bit 不用動
    *out4 = inD % inA; 輸出是 64 bit 的資料, 會把 inA char 擴成 64 bit 再來處理
}
```

結論就是:
在加法來說 當輸出的寬度比輸入的還大的時候 可以用最小運算, 當輸出的寬度比輸入的還小的時候 還是可以運算
在減法來說 會需要先擴充到完整對應的大小才能做運算

## 浮點數和雙精度

- 單精度 32 位 (4 Bytes)
24 位小數
8 位指數
- 雙精度 64 位 (8 Bytes)
53 位小數
11 位指數

[說明](https://docs.xilinx.com/r/zh-CN/ug1399-vitis-hls/%E6%B5%AE%E7%82%B9%E4%B8%8E%E5%8F%8C%E7%B2%BE%E5%BA%A6)

## 浮點累加器和 MAC

不是很了

## 複合資料類型

HLS 支持對以下類型作綜合
1. struct
2. enum
3. union

### struct 的情況

程式碼中的 struct 默認情況下是解聚 (disaggregated) 狀態
這些 struct 解聚為對應每個變數的獨立對象 建立的數量和類型取決於 srtuct 本身的内容
假設: srtuct array 當多個陣列來實現, 每個 srtuct 成員都具有獨立的陣列

```c++
typedef struct {
    unsigned short varA; // 2 Bytes = 16 bit
    unsigned char varB[4]; // 1 Bytes = 8 bit
} data_t;

data_t struct_port(data_t i_val, data_t *i_pt, data_t *o_pt);

在接口上聚合 struct 會有單個 48  bit 端口(16+32=48), 其中包含 16 bit 的 varA 和 4x8 bit 的 varB。
```

### C++的類別和模板

他這邊提供了一個濾波器的範例 示範對運算符號重載跟類別, 模板的程式碼 我有放在 192.168.199.2 上

AMD 不建議使用全域變數, 會對最佳化指令有影響, 導致不能最佳化

越來越天書了...

模板的範例

