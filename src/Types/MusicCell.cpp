#include "Types/MusicCell.hpp"

#include <imgui.h>

#include "UI/UIHelper.hpp"

MusicCell::MusicCell(AudioMetadata metadata)
    :metadata(metadata), albumCover(UIHelper::loadImageFromMetadata(metadata.tagAlbumCover)) {}

void MusicCell::render() {
    ImVec2 windowSize = ImGui::GetWindowSize();

    ImGui::BeginChild(metadata.title.c_str(), ImVec2(windowSize.x - 10, 150));

    if (!metadata.tagAlbumCover.isEmpty()) {
        ImGui::Image((ImTextureID)(intptr_t)albumCover.texture, ImVec2(150, 150));
    }

    ImGui::SameLine();

    ImGui::Text(metadata.title.c_str());

    ImGui::EndChild();
}