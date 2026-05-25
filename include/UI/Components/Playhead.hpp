#pragma once

#include "Types/AudioMetadata.hpp"
#include "Types/Image.hpp"
#include "Util/Singleton.hpp"
#include "UI/UIHelper.hpp"

class Playhead : public Singleton<Playhead> {
public:
    void render();

    void setSong(AudioMetadata metadata) {
        this->currentSong = metadata;
        if (!metadata.tagAlbumCover.isEmpty()) {
            this->albumCover = UIHelper::loadImageFromMetadata(metadata.tagAlbumCover);
        }
    }

private:
    AudioMetadata currentSong;
    Image albumCover;
};