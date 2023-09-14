# FPGA的 3 大程式範例

## 生產者使用者

就是併發的概念 有一個Excel檔案 裡面有100欄 我要處理每一欄的資料後 再寫到別的檔案存起來
假設 處理一欄要2秒 寫入要1秒 總共要花300秒

要提高吞吐量的話就是把"處理"跟"寫入"一起做 

![生產者使用者](/img/生產者使用者.png "生產者使用者")

## 串流
就是緩衝器 這邊稍微介紹一下 HLS stream types (fifo, pipo, shared, unsynq)

- FIFO - 先進先出 需要注意 FIFO 只是預先定義大小/深度的Queue

- PIPO(乒乓緩衝) - swaps automatically with handshaking. It can be setup 2 ways - split or merge. Split will use 2 BRAMS of size N with 2 ports for producer and 2 ports for consumer. Merge will use 1 BRAM of size depth * N with 1 port for producer and 1 port for consumer. The user can control this setup with the bind_storage option in the pragma or the scheduler will handle it depending on the code.

我的解讀 一塊餅乾 分成2部份 上面給生產者 下面給使用者 中間的夾心(參數設定或是硬體特性)會分配給的速度 不會死鎖

![PIPO](/img/PIPO.png "PIPO")

下面2個不常用 先不管

- SHARED - is PIPO-like with handshaking but the shared memory will be setup as 1 BRAM of size N. There is no memory-expansion like the PIPO. The depth, d, is what will control the synchronization of the SHARED type. The depth will make sure that iteration i+d of producer does not start before the consumer has read at iteration i. This means the code of the producer is allowed to reuse the memory only after d iterations, or there will be a cosim mismatch.

- UNSYNC - the user controls the size of shared memory and the synchronization. It's the most difficult to use as there's no memory expansion, no synchronization, no handshaking, just user controlled. It's setup as 1 BRAM, size N, 1 port each.

這邊的硬體技術會應用在螢幕上 例如144Hz的螢幕 要時時高清不掉幀

## 流水線

這邊用圖說明

![流水線](/img/流水線.png "流水線")

## 組合3種範例

[官方例子](https://docs.xilinx.com/r/zh-CN/ug1399-vitis-hls/%E7%BB%84%E5%90%88%E4%B8%89%E7%A7%8D%E8%8C%83%E4%BE%8B)

就是使出流水拍打(數據流) (#pragma HLS dataflow) 會幫你優化就對了

[DataFlow的概念](https://zhuanlan.zhihu.com/p/376273195)

## 總結

用一般軟體的思維去寫FPGA的轉換, 需要先考慮, 我們的整體架構, 甚麼時間點該做甚麼事要有時間線, 這一分要有資料 下一秒處理資料, 哪邊可以並行處理

哪邊不行, 哪邊算法可以變模組, 但是又不能有太多的耦合, 要提升效率的化 緩衝之間的設計也要考量
