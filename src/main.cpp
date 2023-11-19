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
#include "ui/LyricUI.h"
#include "IRrecv.h"

#define OLED_SPI_CS 5
#define OLED_SPI_DC 26
#define OLED_SPI_RESET 22
#define OLED_SPI_MOSI 23
#define OLED_SPI_CLK  18
#define SD_SPI_SCK 33
#define SD_SPI_MISO 32
#define SD_SPI_MOSI 25
#define SD_SPI_CS 13
#define IRRemote 35

SPIClass H(VSPI);
SPIClass V(HSPI);
U8G2_SH1106_128X64_NONAME_F_4W_HW_SPI u8g2(U8G2_R0, OLED_SPI_CS, OLED_SPI_DC, OLED_SPI_RESET);
Preferences preferences;
Player player;
UI *currentUI;
IRrecv iRrecv(IRRemote);
decode_results results;

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

    player.setup();
    player.scanSD();

    for (const Song &s: player.playlist) {
        Serial.println(s.name.c_str());
    }

    // 初始化按钮
    pinMode(35, INPUT_PULLUP);
    pinMode(34, INPUT_PULLUP);
    pinMode(39, INPUT_PULLUP);

    // 初始化红外遥控
    iRrecv.enableIRIn();

    player.play(preferences.getInt("track",0));

    currentUI = new LyricUI();
    currentUI->setup();

    xTaskCreatePinnedToCore([](void* args) {
        while (true) {
            if(audio.isRunning()){
                audio.loop();
            }
            vTaskDelay(0);
        }
    }, "audio", 10000, NULL, 1, NULL, 0);

    xTaskCreatePinnedToCore([](void* args) {
        while (true) {
            currentUI->loop();
            if(iRrecv.decode(&results)){
                currentUI->ir(results);
                iRrecv.resume();
            }
            vTaskDelay(0);
        }
    }, "render", 10000, NULL, 2, NULL, 1);
}

void loop() {
    /*audio.loop();
    currentUI->loop();
    runner.execute();*/
}