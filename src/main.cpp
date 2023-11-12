//
// Created by awalol on 2023/10/15.
//
#include <SPI.h>
#include <SD.h>
#include <vector>
#include "Arduino.h"
#include "Audio.h"
#include "Preferences.h"
#include "Song.h"
#include "ui/UI.h"
#include "Player.h"
#include "U8g2lib.h"
#include "TaskScheduler.h"
#include "ui/LyricUI.h"

#define OLED_SPI_CS 5
#define OLED_SPI_DC 26
#define OLED_SPI_RESET 22
#define OLED_SPI_MOSI 23
#define OLED_SPI_CLK  18
#define SD_SPI_SCK 33
#define SD_SPI_MISO 32
#define SD_SPI_MOSI 25
#define SD_SPI_CS 13

SPIClass H(VSPI);
SPIClass V(HSPI);
U8G2_SH1106_128X64_NONAME_F_4W_HW_SPI u8g2(U8G2_R0, OLED_SPI_CS, OLED_SPI_DC, OLED_SPI_RESET);
Preferences preferences;
Player player;
UI *currentUI;
Task button(2500, TASK_FOREVER, []() {
    currentUI->tick();
});
Scheduler runner;

extern Audio audio;

void setup() {
    Serial.begin(115200);

    // 初始化显示屏
    V.begin(OLED_SPI_CLK, -1, OLED_SPI_MOSI, OLED_SPI_CS);
    SPI = V;
    u8g2.begin();
    u8g2.enableUTF8Print();
    u8g2.setFont(u8g2_font_wqy12_t_gb2312);
    u8g2.drawUTF8(20, 20, "HelloWorld Setup");
    u8g2.sendBuffer();

    // 初始化 SD
    H.begin(SD_SPI_SCK, SD_SPI_MISO, SD_SPI_MOSI, SD_SPI_CS);
    if (!SD.begin(SD_SPI_CS, H)) {
        Serial.println("SD 读取失败");
    }

    // 初始化配置
    preferences.begin("config");

    /*if(SD.exists("/playlist.json")){
        readPlaylist();
        Serial.println("readPlaylist");
    } else{
        scanSD();
        Serial.println("scanSD");
    }*/

    player.setup();

    player.scanSD();

    for (const Song &s: player.playlist) {
        Serial.println(s.name.c_str());
    }

    // 初始化按钮
    pinMode(35, INPUT_PULLUP);
    pinMode(34, INPUT_PULLUP);
    pinMode(39, INPUT_PULLUP);

    // 初始化定时器
    runner.init();
    runner.addTask(button);
    button.enable();

    player.play(preferences.getInt("track",0));

    currentUI = new LyricUI();
    currentUI->setup();

    xTaskCreate([](void *args) {
        while (1) {
            audio.loop();
        }
    }, "audio", 2048, NULL, 1, NULL);

    xTaskCreate([](void *args) {
        while (1) {
            currentUI->loop();
        }
    }, "render", 2048, NULL, 2, NULL);
}

void loop() {
    runner.execute();
}