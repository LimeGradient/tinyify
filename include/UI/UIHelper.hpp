#pragma once

#include <functional>
#include <string>

#include <imgui.h>
#include <SDL3/SDL.h>

#include "Util/Logging.hpp"

class UIHelper {
public:
    static const ImGuiWindowFlags DEFAULT_WINDOW_FLAGS = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar;

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
};