#include <SDL3/SDL.h>

#include <imgui.h>
#include <backends/imgui_impl_sdl3.h>
#include <backends/imgui_impl_sdlrenderer3.h>

#include "Util/Logging.hpp"
#include "UI/UIHelper.hpp"

SDL_Window* window;
SDL_Renderer* renderer;

int main() {
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        return 1;
    }

    float mainScale = SDL_GetDisplayContentScale(SDL_GetPrimaryDisplay());
    if (!SDL_CreateWindowAndRenderer("tinyify", 1280, 720, SDL_WINDOW_RESIZABLE, &window, &renderer)) {
        Logging::error("Couldnt create window or renderer: {}", SDL_GetError());
        return 1;
    }

    SDL_SetRenderLogicalPresentation(renderer, 1280, 720, SDL_LOGICAL_PRESENTATION_DISABLED);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    ImGui::StyleColorsDark();

    ImGuiStyle& style = ImGui::GetStyle();
    style.ScaleAllSizes(mainScale);
    style.FontScaleDpi = mainScale;

    ImGui_ImplSDL3_InitForSDLRenderer(window, renderer);
    ImGui_ImplSDLRenderer3_Init(renderer);

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
        UIHelper::createPanel("MainPanel", windowSize, ImVec2(0, 0), [windowSize]() {
            ImGui::SetCursorPos(ImVec2(0, 0));
            ImGui::BeginChild("MainPanelChildren", windowSize, true, UIHelper::DEFAULT_WINDOW_FLAGS | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoInputs);

            std::string title = "tinyify";
            float titleX = (ImGui::GetWindowSize().x / 2) - (ImGui::CalcTextSize(title.c_str()).x / 2);
            ImGui::Text(title.c_str());
            
            ImGui::Separator();

            ImGui::EndChild();

        }, UIHelper::DEFAULT_WINDOW_FLAGS | ImGuiWindowFlags_NoInputs);

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