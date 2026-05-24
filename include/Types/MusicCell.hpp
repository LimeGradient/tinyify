#pragma once

#include "Types/AudioMetadata.hpp"
#include "Types/Image.hpp"

class MusicCell {
public:
    MusicCell(AudioMetadata metadata);

    void render();

    AudioMetadata metadata;

private:
    Image albumCover;
};