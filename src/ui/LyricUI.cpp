//
// Created by awalol on 2023/10/29.
//

#include <Preferences.h>
#include "LyricUI.h"
#include "lyricparser/Line.h"
#include "U8g2lib.h"
#include "lyricparser/LyricParser.h"
#include "Player.h"

using namespace std;

extern U8G2 u8g2;
extern u_long starts;
extern Preferences preferences;
float cur = 0;
long lastLine;
extern LyricParser lyricParser;
extern Audio audio;
extern String playName;
extern Player player;
extern bool qrc;

void LyricUI::setup() {
    u8g2.setFont(u8g2_font_wqy12_t_gb2312);
}

void LyricUI::loop() {
    float currentTime = 1;
    float duration = 1;
    if(audio.isRunning()){
        currentTime = audio.getAudioCurrentTime();
        duration = audio.getAudioFileDuration();
    }

    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_wqy14_t_gb2312);
    u8g2.drawUTF8(0, u8g2.getMaxCharHeight(), playName.c_str());
    u8g2.drawFrame(0, u8g2.getHeight() - 15, u8g2.getWidth(), 15);
    u8g2.drawBox(0, u8g2.getHeight() - 15,
                 u8g2.getWidth() * float (currentTime / duration), 15);

    if(qrc){
        u_long t = currentTime * 1000;
        Line line = lyricParser.getCurrentLine(t);
        if (line.startTime != lastLine) {
            lastLine = line.startTime;
            cur = 0;
        }
        u8g2.setFont(u8g2_font_wqy12_t_gb2312);

        for(const Word& w : line.words){
            if (w.startTime <= t) {
                int x = w.startX + cur + u8g2.getUTF8Width(w.word.c_str()) * 2;
                if(x >= u8g2.getDisplayWidth()){
                    cur -= x - u8g2.getDisplayWidth();
                }
            }
            u8g2.drawUTF8X2(w.startX + cur,u8g2.getHeight() - 20,w.word.c_str());
        }
    }
    u8g2.sendBuffer();
}

void LyricUI::ir(decode_results results) {
    switch (results.command) {
        case 0x40:
            player.nextTrack();
            break;
        case 0x44:
            player.previousTrack();
            break;
        case 0x15:
            audio.setVolume(audio.getVolume() + 1);
            preferences.putInt("volume",audio.getVolume());
            break;
        case 0x7:
            audio.setVolume(max(audio.getVolume() - 1,0));
            preferences.putInt("volume",audio.getVolume());
            break;
        case 0x43:
            audio.pauseResume();
            break;
        case 0x16:
            player.play(0);
            break;
    }
}