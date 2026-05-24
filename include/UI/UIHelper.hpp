#pragma once

#include <functional>
#include <string>
#include <vector>

#include <SDL3/SDL.h>
#include <imgui.h>
#include <taglib/tbytevector.h>

#include "Types/AudioMetadata.hpp"
#include "Types/Image.hpp"
#include "Types/MusicCell.hpp"

class UIHelper {
public:
    static const ImGuiWindowFlags DEFAULT_WINDOW_FLAGS = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar;

    static void setRenderer(SDL_Renderer* renderer) {
        UIHelper::renderer = renderer;
    }

    static ImVec2 getCenter(ImVec2 windowSize, ImVec2 panelSize) {
        ImVec2 center = ImVec2((windowSize.x - panelSize.x) / 2, (windowSize.y - panelSize.y) / 2);
        return center;
    }

    static void createPanel(std::string id, ImVec2 size, ImVec2 pos, std::function<void()> render, ImGuiWindowFlags flags = 0) {
        ImGui::Begin(id.c_str(), nullptr, flags);
        ImGui::SetWindowSize(size);
        ImGui::SetWindowPos(pos, ImGuiCond_Once);

        render();

        ImGui::End();
    }

    static MusicCell createMusicCell(AudioMetadata audioMetadata);

    static Image loadImageFromPath(std::string imagePath, ImVec2 imgSize = ImVec2(0, 0));
    static Image loadImageFromURL(std::string url, ImVec2 imgSize = ImVec2(0, 0));
    static Image loadImageFromMetadata(TagLib::ByteVector data, ImVec2 imgSize = ImVec2(0, 0));
    static void renderImages();
    static void prepareAudioFileImages(std::vector<AudioMetadata> files, ImVec2 imgSize = ImVec2(0, 0));

private:
    static bool loadTextureFromMemory(const void* data, size_t data_size, SDL_Renderer* renderer, SDL_Texture** out_texture, int* out_width, int* out_height);
    static bool loadTextureFromFile(const char* file_name, SDL_Renderer* renderer, SDL_Texture** out_texture, int* out_width, int* out_height);

    static std::vector<Image> images;
    static SDL_Renderer* renderer;
};