#include "UI/Components/Playhead.hpp"

#include <format>
#include <imgui.h>

#include "Util/AudioManager.hpp"
#include "Util/Logging.hpp"
#include "UI/UIHelper.hpp"
#include "imgui_internal.h"

void Playhead::render() {
    ImVec2 windowSize = ImGui::GetWindowSize();
    ImVec2 playheadSize = ImVec2(windowSize.x, 125);

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(10.0f, 10.0f));
    UIHelper::createPanel("Playhead", playheadSize, ImVec2(0, windowSize.y - playheadSize.y), [this, windowSize, playheadSize]() {
        if (!currentSong.tagAlbumCover.isEmpty()) {
            ImGui::Image((ImTextureID)(intptr_t)albumCover.texture, ImVec2(100, 100));
        }

        auto am = AudioManager::get();

        ImGui::SameLine();

        float cursorPosY = ImGui::GetCursorPosY();
        ImGui::SetCursorPosY(cursorPosY + 37.5 / 2.0f);

        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
        ImGui::BeginChild(std::format("{}_data", currentSong.title).c_str(), ImVec2(playheadSize.x - 150, 100 ), true, ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoScrollbar);
        
        ImGui::Text("%s", currentSong.title.c_str());
        ImGui::Text("%s", currentSong.albumName.c_str());
        ImGui::Text("%s", currentSong.artists.c_str());
        
        if (am->IsPlaying()) {
            float progress = am->GetCurrentPlayheadPosition() / am->GetSongDuration();
            ImGui::ProgressBar(progress, ImVec2(playheadSize.x - 150, 0), "");

            if (ImGui::IsItemClicked(ImGuiMouseButton_Left)) {
                ImVec2 minPos = ImGui::GetItemRectMin();
                ImVec2 maxPos = ImGui::GetItemRectMax();
                ImVec2 mousePos = ImGui::GetMousePos();
                float relativeX = mousePos.x - minPos.x;
                float barWidth = maxPos.x - minPos.x;
                float clickedPercentage = relativeX / barWidth; 
                clickedPercentage = ImClamp(clickedPercentage, 0.0f, 1.0f);
                Logging::info("{}", clickedPercentage);
            }
        }

        ImGui::EndChild();
        ImGui::PopStyleVar();

    }, UIHelper::DEFAULT_WINDOW_FLAGS);
    ImGui::PopStyleVar();
}