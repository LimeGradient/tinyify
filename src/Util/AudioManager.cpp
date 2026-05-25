#include "Util/AudioManager.hpp"

#include "Util/Logging.hpp"

bool AudioManager::Init() {
    if (!MIX_Init()) {
        Logging::error("Couldn't initialize SDL_mixer: {}", SDL_GetError());
        return false;
    }

    this->mixer = MIX_CreateMixerDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, NULL);
    if (!this->mixer) {
        Logging::error("Couldn't create mixer: {}", SDL_GetError());
        return false;
    }

    this->playerTrack = this->CreateTrack();

    return true;
}

MIX_Audio* AudioManager::LoadAudio(std::string filePath) {
    MIX_Audio* audio = MIX_LoadAudio(this->mixer, filePath.c_str(), true);
    if (!audio) {
        Logging::error("Couldn't load audio from {}: {}", filePath, SDL_GetError());
        return nullptr;
    }

    return audio;
}

Track* AudioManager::CreateTrack() {
    Track* track = new Track(this->mixer);

    return track;
}

Track* AudioManager::CreateTrack(MIX_Audio* audio) {
    Track* track = new Track(this->mixer);
    track->SetAudio(audio);

    return track;
}

void AudioManager::PlayAudioFromMetadata(AudioMetadata metadata) {
    auto audio = this->LoadAudio(metadata.path);\
    this->playerTrack->SetAudio(audio);
    this->playerTrack->Play();
}