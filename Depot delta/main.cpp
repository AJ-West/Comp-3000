// Depot delta.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>

// SDL includes
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <SDL3_image/SDL_image.h>

// ImGui includes
#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_sdl3.h>
#include <imgui/backends/imgui_impl_sdlrenderer3.h>

#include "tinyxml2.h"
#include "mapLoader.h"

#include "unitObject.h"
#include "depotObject.h"
#include "convoyObject.h"

#include "renderComponent.h"
#include "buttonComponent.h"
#include "movementComponent.h"
#include "resourceComponent.h"

#include "variables.h"

#include "levelManager.h"

#include "dayCycle.h"

using namespace std;
using namespace tinyxml2;

vector<UnitObj*> unitList;
vector<ConvoyObj*> convoyList;
DepotObj* depot;
GameObject* hoveredUnit = nullptr;

bool dev_mode = false;

//creates the window, renderer and font for the game
void init_SDL_environment() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        cerr << "SDL could not initialize: SDL_ERROR" << SDL_GetError() << endl;
        isRunning = false;
        return;
    }
    if (TTF_Init() == -1) {
        cerr << "SDL_ttf could not initialize: TTF_ERROR" << SDL_GetError() << endl;
        isRunning = false;
        return;
    }
    window = SDL_CreateWindow("Depot delta", screenWidth, screenHeight, SDL_WINDOW_RESIZABLE);
    if (window == nullptr) {
        cerr << "Window cannot be created: SDL_ERROR" << SDL_GetError() << endl;
        isRunning = false;
        return;
    }
    renderer = SDL_CreateRenderer(window, NULL);
    if (renderer == nullptr) {
        cerr << "Renderer cannot be created: SDL_ERROR" << SDL_GetError() << endl;
        isRunning = false;
        return;
    }
    font = TTF_OpenFont("art/BorderWall.ttf", FONT_SIZE);
    if (font == nullptr) {
        cerr << "Font cannot be loaded: TTF_ERROR" << SDL_GetError() << endl;
        isRunning = false;
        return;
    }

    //Can be used for setting resolution (will be useful when adding settings)
	//SDL_SetRenderScale(renderer, zoom, zoom);
    //Used to render consistenly regardless of screensize
	SDL_SetRenderLogicalPresentation(renderer, ResolutionWidth/zoom, ResolutionHeight/zoom, SDL_LOGICAL_PRESENTATION_LETTERBOX);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    isRunning = true;
}

ImGuiIO& init_ImGui_environment() {
    // Initialize ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

    // Setup ImGui style
    ImGui::StyleColorsDark();

    // Initialize ImGui SDL3 and SDL_Renderer backends
    ImGui_ImplSDL3_InitForSDLRenderer(window, renderer);
    ImGui_ImplSDLRenderer3_Init(renderer);

    return io;
}

void clamp_zoom() {
    if (zoom > 2.0f) {
        zoom = 2.0f;
    }
    else if (zoom < 0.5f) {
        zoom = 0.5f;
    }
}

void handleInput(SDL_Event event, ImGuiIO* io, LevelManager* manager) {
    ImGui_ImplSDL3_ProcessEvent(&event);
    if (event.type == SDL_EVENT_QUIT) {
        isRunning = false;
    }
    else if (event.type == SDL_EVENT_MOUSE_WHEEL) {
        //mouse position before zoom
        float mxb = 0.0f;
        float myb = 0.0f;
        getScaledMousePos(&mxb, &myb);
        zoom += event.wheel.y * 0.1; // zoom in or out based on mouse wheel scroll
        clamp_zoom();
        SDL_SetRenderLogicalPresentation(renderer, ResolutionWidth / zoom, ResolutionHeight / zoom, SDL_LOGICAL_PRESENTATION_LETTERBOX);
        //SDL_SetRenderScale(renderer, zoom, zoom);
        //mouse position after zoom
        float mxa = 0.0f;
        float mya = 0.0f;
        getScaledMousePos(&mxa, &mya);
        camera.dimen.x += mxb - mxa;
        camera.dimen.y += myb - mya;
        camera.dimen.w = ResolutionWidth / zoom;
        camera.dimen.h = ResolutionHeight / zoom;
    }
    else if (event.type == SDL_EVENT_KEY_DOWN && event.key.key == SDLK_GRAVE) { // enter/exit dev mode
        dev_mode = !dev_mode;
    }
    else {
        manager->handleInput(event);
    }
}

int main()
{
    //system("pause");
    
    //initiate the environment
    init_SDL_environment();
    ImGuiIO& io = init_ImGui_environment();

	LevelManager manager(renderer);

    //dayCycle cycle;

	Uint32 lastTime = SDL_GetTicks();

    while (isRunning) {
        Uint32 frameStart = SDL_GetTicks();

        //handle input
        SDL_Event event;
        while (SDL_PollEvent(&event) != 0) {
            handleInput(event, &io, &manager);
        }
        Uint32 currentTime = SDL_GetTicks();
        deltaTime = (currentTime - lastTime) / 1000.0f;
        lastTime = currentTime;

        // gets logical size (resolution*zoom)
        int logical_w = 0, logical_h = 0;
        SDL_GetRenderLogicalPresentation(renderer, &logical_w, &logical_h, nullptr);

        // gets window size
        int win_w = 0, win_h = 0;
        SDL_GetWindowSizeInPixels(window, &win_w, &win_h);

        // Update ImGui display metrics
        io.DisplaySize = ImVec2((float)logical_w, (float)logical_h);
        io.DisplayFramebufferScale = ImVec2(
            (float)win_w / (float)logical_w,
            (float)win_h / (float)logical_h
        );

        // Mouse position: SDL3 already returns logical coords
        float mx, my;
        SDL_GetMouseState(&mx, &my);
        io.MousePos = ImVec2(mx/(2*zoom), my/(2*zoom));

        // Start ImGui frame
        ImGui_ImplSDLRenderer3_NewFrame();
        ImGui_ImplSDL3_NewFrame();
        ImGui::NewFrame();

        if (dev_mode) {
            ImGui::SetNextWindowPos(ImVec2(camera.dimen.x, camera.dimen.y), ImGuiCond_Appearing);
            ImGui::SetNextWindowSize(ImVec2(camera.dimen.w, camera.dimen.h), ImGuiCond_Appearing);
            // Create a simple ImGui window
            ImGui::Begin("Dev");
            ImGui::Text("Dev mode settings");
            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
            ImGui::SetCursorPos(ImVec2(400, 0));
            ImGui::Button("Test", ImVec2(100, 150));
            ImGui::End();
        }

        // Render ImGui
        ImGui::Render();
		SDL_RenderClear(renderer);
        manager.render();
        //cycle.update();
        //cycle.render(renderer);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), renderer);
		SDL_RenderPresent(renderer);

        // Frame rate capping
        Uint32 frameTime = SDL_GetTicks() - frameStart;
        if (frameTime < frameDelay) {
            SDL_Delay(frameDelay - frameTime);
        }
    }

	manager.exit();

    // Cleanup
    ImGui_ImplSDLRenderer3_Shutdown();
    ImGui_ImplSDL3_Shutdown();
    ImGui::DestroyContext();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}