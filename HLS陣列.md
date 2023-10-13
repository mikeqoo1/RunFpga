# HLS Array

要給硬體用的話, 不可以用動態分配的方式宣告 Array, 要用靜態的方式宣告
FPGA 上的存儲架構(就是板子裡的記憶體)跟電腦上插的硬碟或記憶體有差別, 訪問自己的很快, 訪問電腦的會有延遲

```c++
int A[N]; // 這個就是移位暫存器
  
for(...) {
  // 移位操作
  for (int i = 0; i < N-1; ++i)
    A[i] = A[i+1];
  A[N] = ...;
  
  // 訪問移位暫存器
  ... A[x] ...
}
```

最優化的當 DataFlow 或展開迴圈的時候 並行 Array 前提是板子上有夠多的暫存器拉

移位暫存器每個周期都能執行一次移位操作 還可以在移位暫存器内任意位置每個周期執行一次隨機讀取 比 FIFO 更靈活

在下列情况下 Array 在 RTL 中可能會有問題

1. 當存儲器(BRAM/LUTRAM/URAM) 來實現時 存儲器端口的數量可能限制對資料的訪問 進而導致 DataFlow 巡迴內出現 II 違例
2. HLS 可能無法正確推斷手動互斥訪問
3. 確保在 RTL 中將只需讀訪問權的 Array 作為 ROM 來實現

Vitis HLS 支持指標 Array 每個指標都只能指向 1 個常量或 1 個常量 Array
Array 大小必須固定 例如:Array[10] 不支持非固定大小的 Array 如:Array[]

## 訪問 Array 和性能

```c++
#include "array_mem_bottleneck.h"

dout_t array_mem_bottleneck(din_t mem[N]) {
  dout_t sum=0;
  int i;
 
  SUM_LOOP:for(i=2;i<N;++i)
    sum += mem[i] + mem[i-1] + mem[i-2];

  return sum;
}
```
Array 作為 RAM 來實現 如果把 RAM 指定為一個 RAM 那麼就不可以對 SUM_LOOP 迴圈做 DataFlow 來實現在每個時鐘週期内處理新的迴圈

嘗試用 II=1 的間隔啟動 SUM_LOOP 進行 DataFlow 將生成以下警告:
```txt
INFO: [SCHED 61] Pipelining loop 'SUM_LOOP'.
WARNING: [SCHED 69] Unable to schedule 'load' operation ('mem_load_2', 
bottleneck.c:62) on array 'mem' due to limited memory ports.
INFO: [SCHED 61] Pipelining result: Target II: 1, Final II: 2, Depth: 3.
```

會有警告的原因是單一個出入口的 RAM 每次週期只能做 1 次讀取或寫入

- SUM_LOOP Cycle1:讀取 mem[i]
- SUM_LOOP Cycle2:讀取 mem[i-1] 對值求和
- SUM_LOOP Cycle3:讀取 mem[i-2] 對值求和

下面是修改後的 Code 把 Array[0]跟[1]獨立在外讀取 這樣在循環裡只要讀 1 次 Array 就好  但是這樣改太累了 也不現實
```c++
#include "mem_bottleneck_resolved.h"

dout_t mem_bottleneck_resolved(din_t mem[N]) {

  din_t tmp0, tmp1, tmp2;
  dout_t sum=0;
  int i;

  tmp0 = mem[0];
  tmp1 = mem[1];
  SUM_LOOP:for (i = 2; i < N; i++) { 
    tmp2 = mem[i];
    sum += tmp2 + tmp1 + tmp0;
    tmp0 = tmp1;
    tmp1 = tmp2;
  }

  return sum;
}
```

這邊介紹 2 種 Vitis HLS 用來更改 Array 實現和訪問的最佳化指令

最嘉化主要分 2 類:

1. "Array Partition" (陣列分區) 各個擊破的概念 拆小一點處理
2. "Array Reshape" (陣列重塑) 把陣列用不同的方式組合 增加可併行度不拆分原始陣列

## Array Partition

Array 可使用 ARRAY_PARTITION 指令來做分區 
Vitis HLS 可提供 3 種類型的分區方法

1. block 原始 Array 分割為原始 Array 的連續元素塊
2. cyclic 原始 Array 分割多個大小相同的塊 這些塊交織成原始 Array 的元素
3. complete 默認是把陣列內容獨立拆分 變成一個一的的暫存器

![陣列分區類型](/img/陣列分區類型.png)

對於多維陣列進行分區的話 可以用 dimension 選項 看要對哪一個維度分區

```c++
void foo (...) {
  int  my_array[10][6][4];
    ......
}
dimension 3 做分區會生成 4 個獨立陣列 對 dimension 1 分區會生成 10 個獨立陣列 如果 dimension 指定 0 則對所有维度分區
```

![多維陣列分區](/img/多維陣列分區.png)

## Array Reshape 看無?

[陣列重塑](https://docs.xilinx.com/r/zh-CN/ug1399-vitis-hls/%E9%98%B5%E5%88%97%E9%87%8D%E5%A1%91)

ARRAY_RESHAPE 指令 (垂直重映射模式來重構陣列 減少所消耗的塊 RAM 的數量 同時提供對資料的並行訪問)

```c++
void foo (...) {
int  array1[N];
int  array2[N];
int  array3[N];
#pragma HLS ARRAY_RESHAPE variable=array1 type=block factor=2 dim=1
#pragma HLS ARRAY_RESHAPE variable=array2 type=cycle factor=2 dim=1
#pragma HLS ARRAY_RESHAPE variable=array3 type=complete dim=1
...
}
```

![陣列重塑](/img/陣列重塑.png)

## 參數上的 Array

假設把 Array 作為 Top Function(頂層函數)的參數時 Vitis HLS 會採用下面的設定

- 存儲器位於偏外, 且 Vitis HLS 會在接口上综合 M_AXI 端口, 以便訪問存儲器, 這是 Vitis 内核流程的默認行為
- 存儲器為標準 RAM, 延遲是 1, 這是 Vivado IP 流程的默認行為 提供地址後經過 1 個時脈周期後, 資料會處在就緒狀態

要配置 Vitis HLS 建造這些端口 要執行下列操作

- 使用 INTERFACE 編譯指令把接口指定為 M_AXI BRAM 或 FIFO 接口
- 使用 INTERFACE 編譯指令的 storage_type 選項, 將 RAM 指定為單端口或雙端口 RAM
- 使用 INTERFACE 編譯指令的 latency 選項, 指定 RAM 延遲
- 使用陣列最優化指令 ARRAY_PARTITION 或 ARRAY_RESHAPE 重新配置陣列結構 從而重新配置 I/O 端口的數量

## Array 接口

INTERFACE 編譯指令搭配 storage_type=<value>選項來定義 Array 的類型 value 預設是單端口的 RAM, 雙端口可以縮短起時間或減少延遲

ARRAY_PARTITION 和 ARRAY_RESHAPE 編譯指令可重配置接口上的 Array, Array 可分區為多個小的 Array, 並且在各自接口單得實現 可以增加並行的可行性但是硬體資源會變多

```c++
#include "array_RAM.h"

void array_RAM (dout_t d_o[4], din_t d_i[4], didx_t idx[4]) {
  int i;

  For_Loop: for (i=0;i<4;i++) {
    d_o[i] = d_i[idx[i]];
  }
}
```

https://docs.xilinx.com/r/zh-CN/ug1399-vitis-hls/%E9%98%B5%E5%88%97%E6%8E%A5%E5%8F%A3

## FIFO 接口

如果把 Array 當 FIFO 實現, 要確保 Array 的訪問順序
d_i 和 d_o 會被當作 FIFO, 如果 idx[i]是順序的話可以成立, 如果是隨機值, 那麼在模擬階段可能就會失敗了

```c++
#include "array_FIFO.h"

void array_FIFO (dout_t d_o[4], din_t d_i[4], didx_t idx[4]) {
  int i;
#pragma HLS INTERFACE mode=ap_fifo port=d_i
#pragma HLS INTERFACE mode=ap_fifo port=d_o
  For_Loop: for (i=0;i<4;i++) {
    d_o[i] = d_i[idx[i]];
  }
}
```

以下規則適合用在把 Array 當 FIFO 的時候:

- 在 Loop 或函數中, Array 必須只能是讀取或寫入 這是變為 FIFO 特徵的點對點連接
- Array 讀取與 Array 寫入的順序必須相同 針對 FIFO 通道不支持隨機訪問, 必須遵守先進先出的程序内使用 Array

https://docs.xilinx.com/r/zh-CN/ug1399-vitis-hls/FIFO-%E6%8E%A5%E5%8F%A3

## 存儲器映射接口

Vitis HLS 允許 Array 指定為 M_AXI 接口, 這樣會把存儲器變為外部的, 而不是使用內部的, 所以訪問外部的會花更多時間

Vitis HLS 會執行自動突發最佳化, 有效讀取或寫入外部存儲器 突發是一種最優化方式, 會嘗試用智能方式聚集對 DDR 的存儲器訪問操作, 盡可能提升吞吐量或減少延遲突發是可對内核執行的諸多最優化操作之一, 突發通常能夠實現 4 到 5 倍的提升 DDR 端口上存在競爭(多個相互競爭的内核)


[AXI 介紹章節](https://docs.xilinx.com/r/zh-CN/ug1399-vitis-hls/AXI-%E7%B3%BB%E7%BB%9F%E6%80%A7%E8%83%BD%E6%9C%80%E4%BC%98%E5%8C%96)

AXI4 協議的突發功能特性能夠通過在單個操作内, 在全局存储器上讀取或寫入多個資料塊來提升存儲功能的吞吐量, 資料越大, 吞吐量越高
這種指標的計算方式如下: ((傳送的大小)*(內核頻率)/(時間))
最大内核接口 512 位, 如果内核編譯假設 300 MHz 頻率, 理論上每個 DDR 可達成(512* 300 MHz)/1 秒 = ~17 GB/s

https://docs.xilinx.com/r/zh-CN/ug1399-vitis-hls/%E5%AD%98%E5%82%A8%E5%99%A8%E6%98%A0%E5%B0%84%E6%8E%A5%E5%8F%A3

## Array 的初始化和復位

AMD 建議用 static 的方式來宣告 Array 這樣可以確保 Vitis HLS 以 RTL 中的存儲器來實做 Array, 還允許使用 static 類型的默認初始化行為

```c++
/*
每次執行時, 用於實現 coeff 的 RAM 都會隨這些值一起載入,
對於單端口 RAM, 此操作耗時 8 個週期, 對於 1024 陣列也就需要 1024 個週期, 在此期間沒辦法執行任何依賴 coeff 的運算
*/
int coeff[8] = {-2, 8, -4, 10, 14, 10, -4, 8, -2};
/*
用靜態的話, 會保留上次執行的值, 會跟 RTL 中的存儲器行為一樣

如果變數包含 static, Vitis HLS 會對 RTL 設計和 FPGA 比特流中的變數做初始化
不用經歷多個時脈週期來初始化存儲器, 可確保大型存儲器初始化不會產生任何運算開銷
*/
static int coeff[8] = {-2, 8, -4, 10, 14, 10, -4, 8, -2};
```

[比特流](https://xilinx.eetrend.com/blog/2022/100566476.html)

RTL 配置命令 config_rtl -reset 可指定在應用復位後, 靜態變數是否返回初始值 但是這不是默認選項, 使用 reset state 或 all 時, 會強制把所有用
RAM 實現的 Array 做復位回初始狀態 可能導致 RTL 設計中出现 2 個非常不利的條件:

- 不同於上電初始化, 顯式復位要求 RTL 設計對 RAM 中的每個地址進行迭代以設定值：如果 N 較大, 可能導致時脈明顯增加, 並增加實現復位所需的面積資源
- 在設計中每個 Array 中都添加復位

復位這邊不是很懂...

## 實現 ROM

如果 Array 只有讀取, 建議使用 const 宣告, 因為 Vitis HLS 就不用一直分析是不是要使用 ROM. ROM 的自動分析一般來說是局部分析, static Array 先完全寫入後讀取, 且不再寫入. 在代碼中遵循下列方法有助於分析 ROM：
- 在使用 Array 的函數中盡早初始化 Array.
- 將寫入的操作组合在一起.
- 勿將 array(ROM) 初始化寫入操作跟非初始化代碼合在一起.
- 勿將不同值存儲到相同 Array 元素中.
- 元素值計算不得依賴於除初始化循環計數器變量以外的任何非常量設計變量. (應該是計算的時候 不能把迴圈的 i 當作變量吧)

## 網路資料

[HLS 之接口設計](https://blog.csdn.net/qq_53144843/article/details/127708508)