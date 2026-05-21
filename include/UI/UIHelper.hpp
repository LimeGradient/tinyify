#pragma once

#include <functional>
#include <string>

#include <SDL3/SDL.h>
#include <imgui.h>

#include "SDL3/SDL_render.h"
#include "Util/Logging.hpp"

class UIHelper {
public:
    static const ImGuiWindowFlags DEFAULT_WINDOW_FLAGS = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar;

    static bool loadTextureFromMemory(const void* data, size_t data_size, SDL_Renderer* renderer, SDL_Texture** out_texture, int* out_width, int* out_height);
    static bool loadTextureFromFile(const char* file_name, SDL_Renderer* renderer, SDL_Texture** out_texture, int* out_width, int* out_height);

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

    static void renderImage(std::string imagePath, SDL_Renderer* renderer) {
        SDL_Texture* texture;
        int w, h;
        bool ret = loadTextureFromFile(imagePath.c_str(), renderer, &texture, &w, &h);
        IM_ASSERT(ret);

        ImGui::Image((ImTextureID)(intptr_t)texture, ImVec2((float)w, float(h)));
    }
};