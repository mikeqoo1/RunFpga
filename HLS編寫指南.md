# FPGA的 3 大程式範例

## 生產者使用者

有一個Excel檔案 裡面有100欄 我要處理每一欄的資料後 再寫到別的檔案存起來
假設 處理一欄要2秒 寫入要1秒 總共要花300秒

要提高吞吐量的話就是把"處理"跟"寫入"一起做 



## 串流資料
https://docs.xilinx.com/r/zh-CN/ug1399-vitis-hls/%E4%B8%B2%E6%B5%81%E6%95%B0%E6%8D%AE%E8%8C%83%E4%BE%8B
https://support.xilinx.com/s/question/0D54U00006am5AOSAY/hls-stream-types-fifo-pipo-shared-unsynq-what-do-they-do?language=zh_CN
就是用緩衝器 這邊稍微介紹一下 HLS stream types (fifo, pipo, shared, unsynq)

- FIFO - 先進先出

- PIPO - swaps automatically with handshaking. It can be setup 2 ways - split or merge. Split will use 2 BRAMS of size N with 2 ports for producer and 2 ports for consumer. Merge will use 1 BRAM of size depth * N with 1 port for producer and 1 port for consumer. The user can control this setup with the bind_storage option in the pragma or the scheduler will handle it depending on the code.

我的解讀 一塊餅乾 分成2部份 上面給生產者 下面給使用者 中間的夾心會分配給的速度 不會死鎖

https://docs.xilinx.com/r/en-US/wp554-high-performance-design/PIPO-and-hls-Stream_of_Blocks


- SHARED - is PIPO-like with handshaking but the shared memory will be setup as 1 BRAM of size N. There is no memory-expansion like the PIPO. The depth, d, is what will control the synchronization of the SHARED type. The depth will make sure that iteration i+d of producer does not start before the consumer has read at iteration i. This means the code of the producer is allowed to reuse the memory only after d iterations, or there will be a cosim mismatch.

- UNSYNC - the user controls the size of shared memory and the synchronization. It's the most difficult to use as there's no memory expansion, no synchronization, no handshaking, just user controlled. It's setup as 1 BRAM, size N, 1 port each.


## 流水線

