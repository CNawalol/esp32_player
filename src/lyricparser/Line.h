//
// Created by awalol on 2023/9/30.
//

#ifndef LYRICPARSER_LINE_H
#define LYRICPARSER_LINE_H

#include "string"
#include "Word.h"
#include "vector"

using namespace std;

struct Line {
    long startTime;
    long duration;
    vector<Word> words;
};

#endif
