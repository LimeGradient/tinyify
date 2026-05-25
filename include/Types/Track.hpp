#pragma once

#include <SDL3_mixer/SDL_mixer.h>

class Track {
public:
    Track(MIX_Mixer* mixer) {
        this->mixerTrack = MIX_CreateTrack(mixer);
    }

    void Play() {
        isPlaying = true;
        MIX_SetTrackAudio(this->mixerTrack, this->audio);
        MIX_PlayTrack(this->mixerTrack, 0);
    }

    void Pause() {
        isPlaying = false;
        MIX_PauseTrack(this->mixerTrack);
    }

    void SetAudio(MIX_Audio* audio) {
        this->audio = audio;
    }

    MIX_Audio* GetAudio() {
        return this->audio;
    }

    void SetVolume(float volume) {
        MIX_SetTrackGain(this->mixerTrack, volume);
    }

    MIX_Track* GetMixerTrack() {
        return this->mixerTrack;
    }

    bool IsPlaying() {
        return isPlaying;
    }

private:
    bool isPlaying = false;
    MIX_Track* mixerTrack;
    MIX_Audio* audio;
    float volume;
};