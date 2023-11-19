//
// Created by awalol on 2023/10/29.
//

#ifndef ESP32_MP3_LYRICUI_H
#define ESP32_MP3_LYRICUI_H

#include "UI.h"

class LyricUI : public UI{
    void setup() override;
    void loop() override;
    void ir(decode_results results) override;
};


#endif //ESP32_MP3_LYRICUI_H
