#pragma once

#include <string>
#include <vector>
#include <taglib/tbytevector.h>

#include "Image.hpp"

class AudioMetadata {
public:
    AudioMetadata() = default;
    AudioMetadata(std::string title, std::string albumName, std::string artists, std::vector<unsigned char> albumCover)
        : title(title), albumName(albumName), artists(artists), albumCover(albumCover) {}
    
    AudioMetadata(std::string title, std::string albumName, std::string artists, TagLib::ByteVector tagAlbumCover)
        : title(title), albumName(albumName), artists(artists), tagAlbumCover(tagAlbumCover) {}

    std::string title;
    std::string albumName;
    std::string artists;
    std::vector<unsigned char> albumCover;
    TagLib::ByteVector tagAlbumCover;
};