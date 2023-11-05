//
// Created by awalol on 2023/10/29.
//

#ifndef ESP32_MP3_PLAYER_H
#define ESP32_MP3_PLAYER_H

#include "Audio.h"
#include "Song.h"
#include "vector"

using namespace std;

class Player {
public:
    vector<Song> playlist;
    void setup();
    void scanSD();
    void play(int i);
    static void nextTrack();
private:
    static bool endsWith(const std::string &str, const std::string& suffix);
};

#endif //ESP32_MP3_PLAYER_H
