# 重置您的 ESP8266
**除特殊情况外，您无须屡次重置 ESP8266**
## 方法1（**强烈推荐**）
1. 安装 [Python 2 或 Python 3](https://www.python.org/downloads/)
2. 运行 `pip install esptool`
3. 运行 esptool.exe --port ***端口(例如： COM1)*** erase_flash

## 方法2

打开 `Reset_Sketch.ino`，然后上传。

## 方法3

直接刷入 `reset_` 文件。

## 方法4

1. 1MB 的 ESP8266 ，请把 `blank_1MB.bin` 刷入 0x000000。
2. 4MB 的 ESP8266 ，请把 `blank_1MB.bin` 刷入0x000000, 0x100000, 0x200000 和 0x300000。
