#ifndef SCRATCH_SOUND_H
#define SCRATCH_SOUND_H
#include <SDL2/SDL_mixer.h>
#include <string>
#include <vector>
using namespace std;
struct SoundClip {
    string name;
    string path;
    Mix_Chunk* chunk = nullptr;
    int volume = 100; // 0 to 100

    bool load(const string& p) {
        path = p;
        chunk = Mix_LoadWAV(p.c_str());
        if (!chunk) { SDL_Log("Mix_LoadWAV error: %s", Mix_GetError()); return false; }
        return true;
    }
    void play() {
        if (!chunk) return;
        Mix_VolumeChunk(chunk, (int)(volume / 100.0f * MIX_MAX_VOLUME));
        Mix_PlayChannel(-1, chunk, 0);
    }
    void free() {
        if (chunk) { Mix_FreeChunk(chunk); chunk = nullptr; }
    }
};

struct SoundManager {
    vector<SoundClip> clips;
    int globalVolume = 100;

    bool addClip(const string& name, const string& path) {
        SoundClip sc;
        sc.name = name;
        if (!sc.load(path)) return false;
        sc.volume = globalVolume;
        clips.push_back(sc);
        return true;
    }

    void play(const string& name) {
        for (auto& c : clips)
            if (c.name == name) { c.play(); return; }
    }

    void stopAll() { Mix_HaltChannel(-1); }

    void setVolume(int v) {
        globalVolume = max(0, min(100, v));
        Mix_Volume(-1, (int)(globalVolume / 100.0f * MIX_MAX_VOLUME));
    }

    void changeVolume(int delta) { setVolume(globalVolume + delta); }

    void destroy() {
        for (auto& c : clips) c.free();
        clips.clear();
    }
};

SoundManager soundMgr;

bool initSound() {
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        SDL_Log("SDL_mixer error: %s", Mix_GetError());
        return false;
    }
    return true;
}

void closeSound() {
    soundMgr.destroy();
    Mix_CloseAudio();
}
#endif //SCRATCH_SOUND_H
