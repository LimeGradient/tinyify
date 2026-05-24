#include "Types/MusicCell.hpp"

#include <format>
#include <imgui.h>

#include "UI/UIHelper.hpp"
#include "Util/Logging.hpp"

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
    ImGui::SetCursorPosY(cursorPosY / 2.0f);

    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
    ImGui::BeginChild(std::format("{}_data", metadata.title).c_str(), ImVec2(300, 50 ), true, ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoScrollbar);
    ImGui::Text("%s", metadata.title.c_str());
    ImGui::Text("%s", metadata.albumName.c_str());
    ImGui::Text("%s", metadata.artists.c_str());
    ImGui::EndChild();
    ImGui::PopStyleVar();

    ImGui::EndChild();

    if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(0)) {
        Logging::info("Clicked music cell: {}", metadata.title);
    }
}