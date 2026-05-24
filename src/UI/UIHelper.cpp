#include "UI/UIHelper.hpp"

#include <SDL3/SDL.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "Util/Logging.hpp"
#include "Util/WebManager.hpp"

std::vector<Image> UIHelper::images;
SDL_Renderer* UIHelper::renderer;

MusicCell UIHelper::createMusicCell(AudioMetadata audioMetadata) {
    ImVec2 windowSize = ImGui::GetWindowSize();

    ImGui::BeginChild(audioMetadata.title.c_str(), ImVec2(windowSize.x - 10, 150));

    if (!audioMetadata.tagAlbumCover.isEmpty()) {
        auto image = UIHelper::loadImageFromMetadata(audioMetadata.tagAlbumCover);
        ImGui::Image((ImTextureID)(intptr_t)image.texture, ImVec2(150, 150));
    }

    ImGui::SameLine();

    ImGui::Text(audioMetadata.title.c_str());

    ImGui::EndChild();
}

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

Image UIHelper::loadImageFromPath(std::string imagePath, ImVec2 imgSize) {
    SDL_Texture* texture;
    int w, h;
    bool ret = loadTextureFromFile(imagePath.c_str(), UIHelper::renderer, &texture, &w, &h);
    IM_ASSERT(ret);

    int id = 0;
    if (!images.empty()) {
        id = images.back().id + 1;
    }

    Image image = Image(
        id,
        (imgSize.x > 0) ? imgSize.x : w,
        (imgSize.y > 0) ? imgSize.y : h,
        texture    
    );
    return image;
}

Image UIHelper::loadImageFromURL(std::string url, ImVec2 imgSize) {
    WebManager::ImageBuffer buffer;
    if (WebManager::get().fetchImage(url, buffer)) {
        SDL_Texture* texture;
        int w, h;
        bool ret = loadTextureFromMemory(buffer.data.data(), buffer.data.size(), UIHelper::renderer, &texture, &w, &h);
        IM_ASSERT(ret);

        int id = 0;
        if (!images.empty()) {
            id = images.back().id + 1;
        }

        Image image = Image(
            id,
            (imgSize.x > 0) ? imgSize.x : w,
            (imgSize.y > 0) ? imgSize.y : h,
            texture    
        );
        return image;
    }
}

Image UIHelper::loadImageFromMetadata(TagLib::ByteVector data, ImVec2 imgSize) {
    int width, height, channels;
    unsigned char* imgData = stbi_load_from_memory(
        reinterpret_cast<unsigned char*>(data.data()),
        data.size(),
        &width, &height, &channels, 4
    );

    if (imgData) {
        SDL_Surface* surface = SDL_CreateSurfaceFrom(
            width, height, SDL_PIXELFORMAT_RGBA32, imgData, width * 4
        );

        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

        Image image(
            0,
            (imgSize.x > 0) ? imgSize.x : width,
            (imgSize.y > 0) ? imgSize.y : height,
            texture
        );

        SDL_DestroySurface(surface);
        stbi_image_free(imgData);

        return image;
    }
}

void UIHelper::renderImages() {
    for (auto image : images) {
        ImGui::Image((ImTextureID)(intptr_t)image.texture, ImVec2((float)image.width, (float)image.height));
    }
}