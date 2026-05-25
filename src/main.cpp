#include <SDL3/SDL.h>

#include <imgui.h>
#include <backends/imgui_impl_sdl3.h>
#include <backends/imgui_impl_sdlrenderer3.h>

#include "UI/Fonts/Arial.hpp"
#include "Services/LocalFilesService.hpp"
#include "Util/AudioManager.hpp"
#include "Util/Logging.hpp"
#include "UI/Components/Playhead.hpp"
#include "UI/UIHelper.hpp"

SDL_Window* window;
SDL_Renderer* renderer;

int main() {
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        return 1;
    }

    AudioManager::get()->Init();

    float mainScale = SDL_GetDisplayContentScale(SDL_GetPrimaryDisplay());
    window = SDL_CreateWindow("tinyify", 1280, 720, SDL_WINDOW_RESIZABLE);
    if (!window) {
        Logging::error("Couldn't create window: {}", SDL_GetError());
        return 1;
    }

    SDL_PropertiesID props = SDL_CreateProperties();
    SDL_SetPointerProperty(props, SDL_PROP_RENDERER_CREATE_WINDOW_POINTER, window);
    SDL_SetNumberProperty(props, SDL_PROP_RENDERER_CREATE_PRESENT_VSYNC_NUMBER, 1);
    renderer = SDL_CreateRendererWithProperties(props);
    UIHelper::setRenderer(renderer);
    SDL_DestroyProperties(props);

    SDL_SetRenderLogicalPresentation(renderer, 1280, 720, SDL_LOGICAL_PRESENTATION_DISABLED);
    SDL_SetRenderVSync(renderer, SDL_RENDERER_VSYNC_ADAPTIVE);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.Fonts->AddFontDefault();
    auto arialFont = io.Fonts->AddFontFromMemoryCompressedTTF(arials_compressed_data, sizeof(arials_compressed_size), 16);

    ImGui::StyleColorsDark();

    ImGuiStyle& style = ImGui::GetStyle();
    style.ScaleAllSizes(mainScale);
    style.FontScaleDpi = mainScale;

    ImGui_ImplSDL3_InitForSDLRenderer(window, renderer);
    ImGui_ImplSDLRenderer3_Init(renderer);

    LocalFilesService::get()->init();
    LocalFilesService::get()->loadMusicCells();

    SDL_Event event;
    bool running = true;
    while (running) {
        while (SDL_PollEvent(&event)) {
            ImGui_ImplSDL3_ProcessEvent(&event);
            switch (event.type) {
                case SDL_EVENT_QUIT:
                    running = false;
                    break;
            }
        }

        ImGui_ImplSDLRenderer3_NewFrame();
        ImGui_ImplSDL3_NewFrame();
        ImGui::NewFrame();

        int w, h;
        SDL_GetWindowSize(window, &w, &h);
        ImVec2 windowSize(w, h);
        UIHelper::createPanel("MainPanel", windowSize, ImVec2(0, 0), [windowSize, arialFont]() {
            ImGui::SetCursorPos(ImVec2(0, 0));
            ImGui::BeginChild("MainPanelChildren", windowSize, true, UIHelper::DEFAULT_WINDOW_FLAGS);

            std::string title = "tinyify";
            float titleX = (ImGui::GetWindowSize().x / 2) - (ImGui::CalcTextSize(title.c_str()).x / 2);
            ImGui::Text("%s", title.c_str());
            
            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(10, 20));
            ImGui::Separator();
            
            ImGui::BeginChild("MainPanelMusicCells", ImVec2(windowSize.x - 20, windowSize.y - 180), true, UIHelper::DEFAULT_WINDOW_FLAGS);

            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
            LocalFilesService::get()->renderMusicCells();
            ImGui::PopStyleVar();

            ImGui::EndChild();

            ImGui::PopStyleVar();
            ImGui::EndChild();

            ImGui::PushFont(arialFont);
            Playhead::get()->render();
            ImGui::PopFont();
        }, UIHelper::DEFAULT_WINDOW_FLAGS | ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_NoBringToFrontOnFocus );

        ImGui::Render();
        SDL_SetRenderDrawColorFloat(renderer, 0, 0, 0, SDL_ALPHA_TRANSPARENT_FLOAT);
        SDL_RenderClear(renderer);
        ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), renderer);
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}