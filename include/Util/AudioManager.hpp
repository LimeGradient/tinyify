#pragma once

#include <string>

#include <SDL3_mixer/SDL_mixer.h>

#include "Types/AudioMetadata.hpp"
#include "Types/Track.hpp"
#include "Util/Singleton.hpp"

class AudioManager : public Singleton<AudioManager> {
public:
    bool Init();
    MIX_Audio* LoadAudio(std::string filePath);

    Track* CreateTrack();
    Track* CreateTrack(MIX_Audio* audio);

    void PlayAudioFromMetadata(AudioMetadata metadata);
    bool IsPlaying() {
        return playerTrack->IsPlaying();
    }

    float GetSongDuration() {
        Sint64 frames = MIX_GetAudioDuration(playerTrack->GetAudio());
        return static_cast<float>(MIX_AudioFramesToMS(playerTrack->GetAudio(), frames));
    }

    float GetCurrentPlayheadPosition() {
        Sint64 frames = MIX_GetTrackPlaybackPosition(playerTrack->GetMixerTrack());
        return static_cast<float>(MIX_TrackFramesToMS(playerTrack->GetMixerTrack(), frames));
    }

private:
    MIX_Mixer* mixer;
    Track* playerTrack;
};