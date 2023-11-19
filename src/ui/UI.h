//
// Created by awalol on 2023/10/22.
//

#ifndef ESP32_MP3_UI_H
#define ESP32_MP3_UI_H

#include "IRrecv.h"

class UI {
public:
    virtual void setup(){};
    virtual void loop(){};
    virtual void ir(decode_results results){}
};

#endif //ESP32_MP3_UI_H
