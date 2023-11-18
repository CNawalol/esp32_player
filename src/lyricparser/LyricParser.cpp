//
// Created by awalol on 2023/10/15.
//
#include "LyricParser.h"
#include "string"
#include "Line.h"

using namespace std;

vector<Line> lines;
extern U8G2 u8g2;

vector<Line> LyricParser::parser(string qrc) {
    lines.clear();
    String r = String(qrc.c_str());
    r.replace("\r\n","\n");
    qrc = r.c_str();
    u8g2.setFont(u8g2_font_wqy12_t_gb2312);
    for (size_t pos = 0; pos < qrc.length();) {
        size_t start = qrc.find_first_of("[0");
        if (pos != 0) {
            start = qrc.find_first_of('\n', pos);
        }
        if (start == std::string::npos)
            break;
        size_t end = qrc.find_first_of('\n', start + 1);
        if (end == std::string::npos)
            break;
        string line = qrc.substr(start + 1, end - start - 1);

        long lineStartTime, lineDuration;
        sscanf(line.c_str(), "[%ld,%ld]", &lineStartTime, &lineDuration);

        size_t lte = line.find_first_of(']');

        string wordsAndTime = line.substr(lte + 1);

        Line l;
        l.startTime = lineStartTime;
        l.duration = lineDuration;

        int startX = 0;

        for (size_t pos2 = 0; pos2 < wordsAndTime.length();) {
            size_t s = wordsAndTime.find_first_of("(", pos2);
            size_t e = wordsAndTime.find_first_of(")", s);
            string word = wordsAndTime.substr(pos2, s - pos2);

//            Serial.print(s);Serial.print(" | ");Serial.println(e);
            string td = wordsAndTime.substr(s, e - s);
            long wordStartTime, wordDuration;
            sscanf(td.c_str(), "(%ld,%ld)", &wordStartTime, &wordDuration);
            Word wd;
            wd.startTime = wordStartTime;
            wd.duration = wordDuration;
            wd.word = word;
            wd.startX = startX;
//            Serial.println(word.c_str());
            startX += u8g2.getUTF8Width(word.c_str()) * 2 + 1;
            l.words.emplace_back(wd);

            pos2 = e + 1;
        }
        lines.emplace_back(l);
        pos = start + 1;
    }

    return lines;
}

Line LyricParser::getCurrentLine(long time) {
    Line currentLine = Line();
    for (const Line &line: lines) {
        if (line.startTime <= time) {
            currentLine = line;
        }
    }
    return currentLine;
}

Word LyricParser::getCurrentWord(const Line &line, long time) {
    Word currentWord = Word();
    for (const Word &word: line.words) {
        if (word.startTime <= time) {
            currentWord = word;
        }
    }
    return currentWord;
}