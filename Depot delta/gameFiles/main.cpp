#include "gameFiles/main.h"

int main()
{
    system("pause");
    
    //initiate the environment
    init_SDL_environment();
    ImGuiIO& io = init_ImGui_environment();

	LevelManager manager(renderer);

    //Set up default camera scale for drawing
    camera.xScale = camera.dimen.w / DESIGN_SCALE_X;
    camera.yScale = camera.dimen.h / DESIGN_SCALE_Y;
    
    //set random seed
    srand(static_cast<unsigned int>(time(0)));

    //dayCycle cycle;

	Uint32 lastTime = SDL_GetTicks();

    while (isRunning) {
        Uint32 frameStart = SDL_GetTicks();

        //handle input
        SDL_Event event;
        while (SDL_PollEvent(&event) != 0) {
            handleInput(event, &manager);
        }

        Uint32 currentTime = SDL_GetTicks();
        deltaTime = (currentTime - lastTime) / 1000.0f;
        lastTime = currentTime;

        adjustForZoom(&io);

        renderImGUI(io);

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

	manager.saveOnExit();

    // Cleanup
    ImGui_ImplSDLRenderer3_Shutdown();
    ImGui_ImplSDL3_Shutdown();
    ImGui::DestroyContext();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

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

    //Used to render consistenly regardless of screensize
    SDL_SetRenderLogicalPresentation(renderer, ResolutionWidth / zoom, ResolutionHeight / zoom, SDL_LOGICAL_PRESENTATION_LETTERBOX);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    isRunning = true;
}

//creates ImGui
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

void handleInput(SDL_Event event, LevelManager* manager){
    ImGui_ImplSDL3_ProcessEvent(&event);
    if (event.type == SDL_EVENT_QUIT) { // exit game
        isRunning = false;
    }
    else if (event.type == SDL_EVENT_KEY_DOWN && event.key.key == SDLK_GRAVE) { // enter/exit dev mode
        dev_mode = !dev_mode;
    }
    else {
        manager->handleInput(event);
    }
}

void adjustForZoom(ImGuiIO* io) {
    // gets logical size (resolution*zoom)
    int logical_w = 0, logical_h = 0;
    SDL_GetRenderLogicalPresentation(renderer, &logical_w, &logical_h, nullptr);

    // gets window size
    int win_w = 0, win_h = 0;
    SDL_GetWindowSizeInPixels(window, &win_w, &win_h);

    // Update ImGui display metrics
    io->DisplaySize = ImVec2((float)logical_w, (float)logical_h);
    io->DisplayFramebufferScale = ImVec2(
        (float)win_w / (float)logical_w,
        (float)win_h / (float)logical_h
    );

    // Mouse position: SDL3 already returns logical coords
    float mx, my;
    SDL_GetMouseState(&mx, &my);
    io->MousePos = ImVec2(mx / (2 * zoom), my / (2 * zoom));
}

void renderImGUI(ImGuiIO& io) {
    // Start ImGui frame
    ImGui_ImplSDLRenderer3_NewFrame();
    ImGui_ImplSDL3_NewFrame();
    ImGui::NewFrame();

    // Devmode
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
}