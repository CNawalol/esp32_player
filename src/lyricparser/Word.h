//
// Created by awalol on 2023/9/30.
//

#ifndef LYRICPARSER_WORD_H
#define LYRICPARSER_WORD_H

#include "string"

struct Word {
    long startTime;
    long duration;
    int startX;
    std::string word;
};

#endif
