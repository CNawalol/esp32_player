# ESP32_Player

**⚠ Warning: 屎山快跑**

## 功能

1. 音乐播放
2. 歌词显示（支持逐词）

## 使用方法

音乐放在`music`目录

解密后的qrc歌词放在`qrc`目录

歌词文件名要与音乐文件名一致并以qrc为文件后缀

修改`main.cpp`中的管脚定义

```c++
#define OLED_SPI_CS 5
#define OLED_SPI_DC 26
#define OLED_SPI_RESET 22
#define OLED_SPI_MOSI 23
#define OLED_SPI_CLK  18
#define SD_SPI_SCK 33
#define SD_SPI_MISO 32
#define SD_SPI_MOSI 25
#define SD_SPI_CS 13
```

34 管脚随机切歌

35 管脚下一首

39 管脚恢复到第一首

如果想要精确逐词需要自行修改`ESP32-audioI2S`的`getAudioCurrentTime`返回类型为float

## 未来计划

- [ ] 优化代码，增加代码可读性和运行性能
- [ ] 增加逐词动画
- [ ] 歌曲列表
- [ ] 主页面
- [ ] 设置