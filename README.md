# RunFpga
FPGA 的雜 7 雜 8 東西

## GitHub 官方庫
[Vitis HLS 簡介範例](https://github.com/Xilinx/Vitis-HLS-Introductory-Examples)
包含許多小型程式碼範例，示範良好的設計實踐、編寫規則、常用應用的設計模式以及（最重要的）最優化技巧，從而最大程度提升應用性能。所有範例都包含 README 文件和 run_hls.tcl 脚本以幫助您使用範例程式碼。

[Vitis 加速範例倉庫](https://github.com/Xilinx/Vitis_Accel_Examples)
包含範例，示範 Vitis 工具和平台的各項功能特性。此倉庫提供了小型有效範例，示範與 Vitis 應用加速開發流程的主機程式碼和内核编程相關的具體案例。這些範例中的内核程式碼均可在 Vitis HLS 中直接編譯。

[Vitis 應用加速開發流程方法](https://github.com/Xilinx/Vitis-Tutorials)
提供多種方法，通過這些方法可以學習有關工具流程和應用開發的具體概念，包括將 Vitis HLS 作為獨立應用來使用的方式，以及在 Vitis 自下而上的設計流程中使用該工具的方式。

[Vitis HLS Coding Styles 整理](https://busy-bob.github.io/post/vitis-hls-coding-styles/)

## 整理精華
[HLS 編寫指南](HLS 編寫指南.md)

## 資料連結
[技術文件](https://docs.xilinx.com/)

[HLS stream types 的介紹](https://support.xilinx.com/s/question/0D54U00006am5AOSAY/hls-stream-types-fifo-pipo-shared-unsynq-what-do-they-do?language=zh_CN)

## 要在 Linux 啟動 Vivado 要在 .bashrc 加入下面 2 行
```bash
source /data/tools/Xilinx/Vivado/2022.2/settings64.sh//Vivado 硬體開發
source /data/tools/Xilinx/Vitis_HLS/2022.2/settings64.sh//Vitis_HLS 把 C/C++ To RTL Code
source /data/tools/Xilinx/Vitis/2022.2/settings64.sh //Vitis Unified IDE
source /opt/xilinx/xrt/setup.sh //set up XILINX_XRT
export PLATFORM_REPO_PATHS=<path to platforms> /data/tools/Xilinx/Vitis/2022.2/platforms
export PLATFORM_REPO_PATHS=/opt/xilinx/platforms/xilinx_u50_gen3x16_xdma_5_202210_1 ㄧ
source .bashrc
vivado
```

## Vitas IDE 開不起來
```bash
先刪掉家目錄的東西
rm -rf .Xilinx/Vitas
https://support.xilinx.com/s/question/0D52E000073JXIvSAO/vitis-fails-to-launch-on-ubuntu-2110?language=zh_CN
```

![Vivado](/img/Vivado.png)

## 設定授權

```bash
 Remember to set eth0
 sudo vi /etc/default/grub
 add “net.ifnames=0 biosdevname=0” to GRUB_CMDLINE_LINUX
 sudo grub2-mkconfig
 sudo grub2-mkconfig -o /boot/grub2/grub.cfg
 sudo vi /etc/sysconfig/network-scripts/ifcfg-xxx
 sudo init 6
 nmcli con show
 nmtui
```

[AMD 論壇](https://support.xilinx.com/s/?language=zh_CN)