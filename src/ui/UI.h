//
// Created by awalol on 2023/10/22.
//

#ifndef ESP32_MP3_UI_H
#define ESP32_MP3_UI_H

class UI {
public:
    virtual void setup(){};

    virtual void loop(){};
    virtual void tick(){}
};

#endif //ESP32_MP3_UI_H
