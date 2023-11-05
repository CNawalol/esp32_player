//
// Created by awalol on 2023/10/15.
//

#ifndef ESP32_MP3_LYRICPARSER_H
#define ESP32_MP3_LYRICPARSER_H

#include "Line.h"
#include "string"
#include "U8g2lib.h"

class LyricParser {
public:
    vector<Line> parser(string qrc);
    Word getCurrentWord(const Line &line, long time);
    Line getCurrentLine(long time);
};


#endif //ESP32_MP3_LYRICPARSER_H
