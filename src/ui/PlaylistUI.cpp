//
// Created by awalol on 2023/10/29.
//

#include <vector>
#include "PlaylistUI.h"
#include "U8g2lib.h"
#include "Song.h"
#include "Player.h"

using namespace std;

extern U8G2 u8g2;
extern Player player;

int currentSelect;
int y = 0;

void PlaylistUI::setup() {
    u8g2.clearBuffer();
    currentSelect = 8;
}

void PlaylistUI::loop() {
    u8g2.clearBuffer();
    for(int i = 0;i < player.playlist.size();i++){
        Song song = player.playlist[i];
        if(player.playlist[currentSelect].name == song.name) {
            if(u8g2.getMaxCharHeight() * (currentSelect + 1) - y >= u8g2.getDisplayHeight()){
                y = u8g2.getMaxCharHeight() * (currentSelect + 1) - y - u8g2.getDisplayHeight();
            }
            u8g2.drawBox(0, u8g2.getMaxCharHeight() * currentSelect + 2 - y, u8g2.getDisplayWidth(), u8g2.getMaxCharHeight() + 1);
        }
        u8g2.setFontMode(1);
        u8g2.setDrawColor(2);
        int s = (u8g2.getMaxCharHeight() * (i + 1) - y);
        if(s <= u8g2.getDisplayHeight() && s >= 0){
            Serial.println(song.name.c_str());
            u8g2.drawUTF8(2,s,song.name.c_str());
        }
    }

    u8g2.sendBuffer();
}

void PlaylistUI::tick() {
    /*int b = digitalRead(35);
    Serial.println(b);
    if(b == 1){
        if(currentSelect < player.playlist.size()){
            currentSelect++;
        } else{
            currentSelect = 0;
        }
    }*/
}