#pragma once

#include <SDL3/SDL.h>

class Image {
public:
    Image(int id, int w, int h, SDL_Texture* texture) : id(id), width(w), height(h), texture(texture) {}

    int id;
    int width;
    int height;
    SDL_Texture* texture;
};