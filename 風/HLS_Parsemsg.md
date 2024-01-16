# HLS 解析字串

使用 HLS 來解析字串順便跟軟體的解析來做 benchmark

最大採坑點 HLS 不支援字串的操作
![Alt text](image-1.png)

所以 strncpy strlen 等等操作都需要額實作

實作後的延遲也蠻高的
![Alt text](image.png)

