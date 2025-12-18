#include "levelManager.h"

#include "levelUI.h"

LevelManager::LevelManager(SDL_Renderer* SDL_Renderer) : renderer(SDL_Renderer)
{
    mapLoader = new MapLoader("maps/test.xml", renderer);
    unitList = mapLoader->getUnitList();
    depot = mapLoader->getDepot();
    convoyList = mapLoader->getConvoyList();
	zombieList = mapLoader->getZombieList();
	vector<GameObject*> allObjects;
	allObjects.insert(allObjects.end(), unitList.begin(), unitList.end());
	allObjects.insert(allObjects.end(), convoyList.begin(), convoyList.end());
	allObjects.insert(allObjects.end(), zombieList.begin(), zombieList.end());
	allObjects.push_back(depot);

    time = new dayCycle();
    UI = new levelUI(renderer, "art/UI/level/Level.png", time);

    selector = new SelectedHandler(allObjects, UI);
}

LevelManager::~LevelManager()
{
}

//On closing the game saves what is needed to be saved
void LevelManager::saveOnExit()
{
	MapSaver saver("maps/test.xml");
    saver.saveFile(unitList, depot, convoyList);
}

// Handles user input
void LevelManager::handleInput(SDL_Event event)
{
    if (event.type == SDL_EVENT_MOUSE_MOTION) {
		selector->checkHover(event);
    }
    else if (event.type == SDL_EVENT_MOUSE_WHEEL) {
        zoomChange(event);
    }
    else if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
        if (event.button.button == SDL_BUTTON_LEFT) {
            if (UI->checkClickInput()) {
                textInput = true;
                return;
            }
            textInput = false;
            selector->checkClick();
        }
        else if (event.button.button == SDL_BUTTON_RIGHT) {
            if (!textInput) {
                selector->rightClick();
            }
        }
    }
    else if (event.type == SDL_EVENT_KEY_DOWN) {
        if (textInput) {
            UI->textInput(event.key.key);
            return;
        }
        camera.keyDown(event.key.key);
    }
    else if (event.type == SDL_EVENT_KEY_UP) {
        camera.keyUp(event.key.key);
    }
}

//Renders the screen
void LevelManager::render()
{
    time->update();
	mapLoader->renderTileMap(renderer);
    depot->Update();
    hoveredUnit = nullptr;
    for (auto& unit : unitList) {
        unit->Update();
        if (unit->getHovering()) {
            hoveredUnit = unit;
        }
    }
    for (auto& convoy : convoyList) {
        convoy->Update();
        if (convoy->getHovering()) {
            hoveredUnit = convoy;
        }
    }
    for (auto& zombie : zombieList) { zombie->Update(); }
    camera.update();
    if (hoveredUnit) {
        UI->renderResourceHover();
        hoveredUnit->renderHover(renderer);
    }
    UI->render();
    UI->renderTime();
    depot->renderResources(renderer);
}


//Handles zoom change
void LevelManager::zoomChange(SDL_Event event) {
    //mouse position before zoom
    float mxb = 0.0f;
    float myb = 0.0f;
    getScaledMousePos(&mxb, &myb);
    zoom += event.wheel.y * 0.1; // zoom in or out based on mouse wheel scroll
    clampZoom();
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

// Keeps zoom within ok range
void LevelManager::clampZoom(){
    if (zoom > 2.0f) {
        zoom = 2.0f;
    }
    else if (zoom < 0.5f) {
        zoom = 0.5f;
    }
}