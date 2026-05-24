#include "Types/MusicCell.hpp"

#include <format>
#include <imgui.h>

#include "UI/UIHelper.hpp"

MusicCell::MusicCell(AudioMetadata metadata)
    :metadata(metadata), albumCover(UIHelper::loadImageFromMetadata(metadata.tagAlbumCover)) {}

void MusicCell::render() {
    ImVec2 windowSize = ImGui::GetWindowSize();
    
    ImGui::BeginChild(metadata.title.c_str(), ImVec2(windowSize.x - 10, 150));

    if (!metadata.tagAlbumCover.isEmpty()) {
        ImGui::Image((ImTextureID)(intptr_t)albumCover.texture, ImVec2(50, 50));
    }

    ImGui::SameLine();

    float cursorPosY = ImGui::GetCursorPosY();
    ImGui::SetCursorPosY(cursorPosY + 12.5 / 2.0f);

    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
    ImGui::BeginChild(std::format("{}_data", metadata.title).c_str(), ImVec2(500, 100));
    ImGui::Text(metadata.title.c_str());
    ImGui::Text(metadata.albumName.c_str());
    ImGui::Text(metadata.artists.c_str());
    ImGui::EndChild();
    ImGui::PopStyleVar();

    ImGui::EndChild();
}