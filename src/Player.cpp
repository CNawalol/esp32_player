//
// Created by awalol on 2023/10/29.
//

#include <Preferences.h>
#include "Player.h"
#include "SD.h"
#include "lyricparser/LyricParser.h"

#define I2S_DOUT 14
#define I2S_LRCK 27
#define I2S_BCK 12
#define I2S_SCK 15

extern Preferences preferences;
extern Player player;
Audio audio;
LyricParser lyricParser;
String playName = "";
bool qrc;
int currentPlay = 0;
extern float m_audioCurrentTime;

void Player::setup() {
    audio.setPinout(I2S_BCK, I2S_LRCK, I2S_DOUT);
    audio.setVolume(preferences.getInt("volume", 12));
    pinMode(I2S_SCK,INPUT_PULLUP);
}

void Player::scanSD() {
    playlist.clear();
    File root = SD.open("/music");
    File file = root.openNextFile();
    while (file) {
        if (endsWith(file.name(), ".mp3")) {
            Song song;
            song.name = file.name();
            song.path = file.path();
            playlist.emplace_back(song);
        }
        file = root.openNextFile();
    }
}

bool Player::endsWith(const std::string &str, const std::string& suffix) {
    if (suffix.length() > str.length()) { return false; }

    return (str.rfind(suffix) == (str.length() - suffix.length()));
}

void audio_info(const char *info){
    Serial.print("info        "); Serial.println(info);
}
void audio_id3data(const char *info){  //id3 metadata
    Serial.print("id3data     ");Serial.println(info);
}

void audio_eof_mp3(const char *info){  //end of file
    Serial.print("eof_mp3     ");Serial.println(info);
    player.nextTrack();
}

void Player::nextTrack() {
    if(currentPlay + 1 >= player.playlist.size()){
        currentPlay = 0;
    } else{
        currentPlay++;
    }
    Serial.print("Next track: ");Serial.println(currentPlay);
    player.play(currentPlay);
}

void Player::previousTrack() {
    if(currentPlay - 1 < 0){
        currentPlay = player.playlist.size() - 1;
    } else{
        currentPlay--;
    }
    Serial.print("Previous track: ");Serial.println(currentPlay);
    player.play(currentPlay);
}

void Player::play(int i) {
    pinMode(I2S_SCK,INPUT_PULLUP);
    audio.stopSong();
    if (!SD.exists(playlist[i].path.c_str())) {
        i = 0;
    }
    audio.connecttoSD(playlist[i].path.c_str());
    String n = String(playlist[i].name.c_str());
    n.replace(".mp3",".qrc");
    String p = "/qrc/" + n;
    if (SD.exists(p.c_str())) {
        try {
            String read = SD.open(p.c_str()).readString();
            lyricParser.parser(read.c_str());
            qrc = true;
        }catch (std::exception& e) {
            qrc = false;
            Serial.print("Read Qrc Error: ");Serial.println(p);
        }
    }
    else {
        qrc = false;
    }
    playName = playlist[i].name.c_str();
    currentPlay = i;
    preferences.putInt("track",i);
    pinMode(I2S_SCK,INPUT_PULLDOWN);
}