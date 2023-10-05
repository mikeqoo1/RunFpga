# HLS Array

要給硬體用的話, 不可以用動態分配的方式宣告Array, 要用靜態的方式宣告
FPGA上的存儲架構(就是板子裡的記憶體)跟電腦上插的硬碟或記憶體有差別, 訪問自己的很快, 訪問電腦的會有延遲

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

最優化的當DataFlow或展開迴圈的時候 並行Array 前提是板子上有夠多的暫存器拉

移位暫存器每個周期都能執行一次移位操作 還可以在移位暫存器内任意位置每個周期執行一次隨機讀取 比FIFO更靈活

在下列情况下 Array在RTL中可能會有問題

1. 當存儲器(BRAM/LUTRAM/URAM) 來實現時 存儲器端口的數量可能限制對資料的訪問 進而導致DataFlow巡迴內出現II違例
2. HLS 可能無法正確推斷手動互斥訪問
3. 確保在RTL中將只需讀訪問權的Array作為ROM來實現

Vitis HLS支持指標Array 每個指標都只能指向1個常量或1個常量Array
Array大小必須固定 例如:Array[10] 不支持非固定大小的Array 如:Array[]

## 訪問Array和性能

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
Array作為RAM來實現 如果把RAM指定為一個RAM 那麼就不可以對SUM_LOOP迴圈做DataFlow來實現在每個時鐘週期内處理新的迴圈

嘗試用II=1的間隔啟動SUM_LOOP進行DataFlow將生成以下警告:
```txt
INFO: [SCHED 61] Pipelining loop 'SUM_LOOP'.
WARNING: [SCHED 69] Unable to schedule 'load' operation ('mem_load_2', 
bottleneck.c:62) on array 'mem' due to limited memory ports.
INFO: [SCHED 61] Pipelining result: Target II: 1, Final II: 2, Depth: 3.
```

會有警告的原因是單一個出入口的RAM 每次週期只能做1次讀取或寫入

- SUM_LOOP Cycle1:讀取mem[i]
- SUM_LOOP Cycle2:讀取mem[i-1] 對值求和
- SUM_LOOP Cycle3:讀取mem[i-2] 對值求和

下面是修改後的Code 把Array[0]跟[1]獨立在外讀取 這樣在循環裡只要讀1次Array就好  但是這樣改太累了 也不現實
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

這邊介紹2種 Vitis HLS 用來更改Array實現和訪問的最佳化指令

最嘉化主要分2類:

1. "Array Partition" (陣列分區) 各個擊破的概念 拆小一點處理
2. "Array Reshape" (陣列重塑) 把陣列用不同的方式組合 增加可併行度不拆分原始陣列

## Array Partition

Array可使用 ARRAY_PARTITION 指令來做分區 
Vitis HLS 可提供 3 種類型的分區方法

1. block 原始Array分割為原始Array的連續元素塊
2. cyclic 原始Array分割多個大小相同的塊 這些塊交織成原始Array的元素
3. complete 默認是把陣列內容獨立拆分 變成一個一的的暫存器

![陣列分區類型](/img/陣列分區類型.png)

對於多維陣列進行分區的話 可以用dimension選項 看要對哪一個維度分區

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

ARRAY_RESHAPE指令 (垂直重映射模式來重構陣列 減少所消耗的塊RAM的數量 同時提供對資料的並行訪問)

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

## 參數上的Array

假設把Array作為Top Function(頂層函數)的參數時 Vitis HLS 會採用下面的設定

- 存儲器位於偏外, 且Vitis HLS會在接口上综合M_AXI端口, 以便訪問存儲器, 這是Vitis内核流程的默認行為
- 存儲器為標準RAM, 延遲是1, 這是Vivado IP流程的默認行為 提供地址後經過1個時脈周期後, 資料會處在就緒狀態

要配置Vitis HLS建造這些端口 要執行下列操作

- 使用 INTERFACE 編譯指令把接口指定為 M_AXI BRAM 或 FIFO 接口
- 使用 INTERFACE 編譯指令的 storage_type 選項, 將 RAM 指定為單端口或雙端口 RAM
- 使用 INTERFACE 編譯指令的 latency 選項, 指定 RAM 延遲
- 使用陣列最優化指令 ARRAY_PARTITION 或 ARRAY_RESHAPE 重新配置陣列結構 從而重新配置 I/O 端口的數量

## Array接口

https://docs.xilinx.com/r/zh-CN/ug1399-vitis-hls/%E9%98%B5%E5%88%97%E6%8E%A5%E5%8F%A3

## FIFO 接口

https://docs.xilinx.com/r/zh-CN/ug1399-vitis-hls/FIFO-%E6%8E%A5%E5%8F%A3

## 存儲器映射接口

https://docs.xilinx.com/r/zh-CN/ug1399-vitis-hls/%E5%AD%98%E5%82%A8%E5%99%A8%E6%98%A0%E5%B0%84%E6%8E%A5%E5%8F%A3