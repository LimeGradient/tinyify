#pragma once

#include "Types/AudioMetadata.hpp"

class MusicCell {
public:
    MusicCell(AudioMetadata metadata);

    void render();

    AudioMetadata metadata;

private:
    Image albumCover;
};