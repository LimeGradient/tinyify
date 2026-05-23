#pragma once

#include <string>

#include <SDL3/SDL.h>

class Image {
public:
    Image() = default;
    Image(int id, int w, int h, SDL_Texture* texture) : id(id), width(w), height(h), texture(texture) {}

    int id;
    int width;
    int height;
    SDL_Texture* texture;
};

class AlbumCover : public Image {
public:
    std::string songTitle;
};