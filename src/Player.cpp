//
// Created by awalol on 2023/10/29.
//

#include <Preferences.h>
#include "Player.h"
#include "SD.h"
#include "ArduinoJson.h"
#include "lyricparser/LyricParser.h"

#define I2S_DOUT 14
#define I2S_LRCK 27
#define I2S_BCK 12

DynamicJsonDocument doc(512);

extern Preferences preferences;
extern Player player;
Audio audio;
LyricParser lyricParser;
String playName = "";
bool qrc;
int currentPlay = 0;
u_long starts = 0;
extern float m_audioCurrentTime;

void Player::setup() {
    audio.setPinout(I2S_BCK, I2S_LRCK, I2S_DOUT);
    audio.setVolume(preferences.getUInt("volume", 12));
}

void Player::scanSD() {
    playlist.clear();
    File playlistFile = SD.open("/playlist.json",FILE_WRITE);
    File root = SD.open("/music");
    File file = root.openNextFile();
    while (file) {
        if (endsWith(file.name(), ".mp3")) {
            JsonObject jo = doc.createNestedObject();
            Song song;
            jo["name"] = song.name = file.name();
            jo["path"] = song.path = file.path();
            playlist.emplace_back(song);
        }
        file = root.openNextFile();
    }
    serializeJson(doc, playlistFile);
    playlistFile.flush();
    playlistFile.close();
}

void readPlaylist(){
    File playlistFile = SD.open("/playlist.json");
    deserializeJson(doc,playlistFile);
    for (JsonObject item : doc.as<JsonArray>()) {
        const char* name = item["name"];
        const char* path = item["path"];
        Song song;
        song.name = name;
        song.path = path;
    }
    playlistFile.close();
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
    Player::nextTrack();
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

void Player::play(int i) {
    if(!SD.exists(playlist[i].path.c_str())){
        i = 0;
    }
    audio.connecttoSD(playlist[i].path.c_str());
    String n = String(playlist[i].name.c_str());
    n.replace(".mp3",".qrc");
    String p = "/qrc/" + n;
    if(SD.exists(p.c_str())){
        String read = SD.open(p.c_str()).readString();
        lyricParser.parser(read.c_str());
        qrc = true;
    }else{
        qrc = false;
    }
    playName = playlist[i].name.c_str();
    currentPlay = i;
    starts = millis();
    preferences.putInt("track",i);
}