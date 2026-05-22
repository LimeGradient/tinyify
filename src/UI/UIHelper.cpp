#include "UI/UIHelper.hpp"

#include <SDL3/SDL.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "Util/WebManager.hpp"

std::vector<Image> UIHelper::images;

bool UIHelper::loadTextureFromMemory(const void* data, size_t data_size, SDL_Renderer* renderer, SDL_Texture** out_texture, int* out_width, int* out_height) {
    int image_width = 0;
    int image_height = 0;
    int channels = 4;
    unsigned char* image_data = stbi_load_from_memory((const unsigned char*)data, (int)data_size, &image_width, &image_height, NULL, 4);
    if (image_data == nullptr) {
        Logging::error("Failed to load image: {}", stbi_failure_reason());
        return false;
    }

    SDL_Surface* surface = SDL_CreateSurfaceFrom(image_width, image_height, SDL_PIXELFORMAT_RGBA32, (void*)image_data, channels * image_width);
    if (surface == nullptr) {
        Logging::error("Failed to create SDL surface: {}", SDL_GetError());
        return false;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (texture == nullptr)
        Logging::error("Failed to create SDL texture: {}", SDL_GetError());

    *out_texture = texture;
    *out_width = image_width;
    *out_height = image_height;

    SDL_DestroySurface(surface);
    stbi_image_free(image_data);

    return true;
}

bool UIHelper::loadTextureFromFile(const char* file_name, SDL_Renderer* renderer, SDL_Texture** out_texture, int* out_width, int* out_height) {
    FILE* f = fopen(file_name, "rb");
    if (f == NULL)
        return false;
    fseek(f, 0, SEEK_END);
    size_t file_size = (size_t)ftell(f);
    if (file_size == -1)
        return false;
    fseek(f, 0, SEEK_SET);
    void* file_data = IM_ALLOC(file_size);
    fread(file_data, 1, file_size, f);
    fclose(f);
    bool ret = loadTextureFromMemory(file_data, file_size, renderer, out_texture, out_width, out_height);
    IM_FREE(file_data);
    return ret;
}

void UIHelper::loadImageFromPath(std::string imagePath, SDL_Renderer* renderer, ImVec2 imgSize) {
    SDL_Texture* texture;
    int w, h;
    bool ret = loadTextureFromFile(imagePath.c_str(), renderer, &texture, &w, &h);
    IM_ASSERT(ret);

    int id = 0;
    if (!images.empty()) {
        id = images.back().id + 1;
    }

    images.push_back(Image(
        id,
        (imgSize.x > 0) ? imgSize.x : w,
        (imgSize.y > 0) ? imgSize.y : h,
        texture    
    ));
}

void UIHelper::loadImageFromURL(std::string url, SDL_Renderer* renderer, ImVec2 imgSize) {
    WebManager::ImageBuffer buffer;
    if (WebManager::get().fetchImage(url, buffer)) {
        SDL_Texture* texture;
        int w, h;
        bool ret = loadTextureFromMemory(buffer.data.data(), buffer.data.size(), renderer, &texture, &w, &h);
        IM_ASSERT(ret);

        int id = 0;
        if (!images.empty()) {
            id = images.back().id + 1;
        }

        images.push_back(Image(
            id,
            (imgSize.x > 0) ? imgSize.x : w,
            (imgSize.y > 0) ? imgSize.y : h,
            texture    
        ));
    }
}

void UIHelper::renderImages() {
    for (auto image : images) {
        ImGui::Image((ImTextureID)(intptr_t)image.texture, ImVec2((float)image.width, (float)image.height));
    }
}