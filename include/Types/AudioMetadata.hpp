#pragma once

#include <string>
#include <vector>
#include <taglib/tbytevector.h>

class AudioMetadata {
public:
    AudioMetadata() : path(""), title(""), albumName(""), artists("") {};
    AudioMetadata(std::string path, std::string title, std::string albumName, std::string artists, std::vector<unsigned char> albumCover)
        : path(path), title(title), albumName(albumName), artists(artists), albumCover(albumCover) {}
    
    AudioMetadata(std::string path, std::string title, std::string albumName, std::string artists, TagLib::ByteVector tagAlbumCover)
        : path(path), title(title), albumName(albumName), artists(artists), tagAlbumCover(tagAlbumCover) {}

    std::string path;
    std::string title;
    std::string albumName;
    std::string artists;
    std::vector<unsigned char> albumCover;
    TagLib::ByteVector tagAlbumCover;
};