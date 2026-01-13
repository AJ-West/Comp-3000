#include "levelManager.h"

#include "levelUI.h"

LevelManager::LevelManager(SDL_Renderer* SDL_Renderer) : renderer(SDL_Renderer)
{
    mapLoader = new MapLoader("maps/test.xml", renderer);
    unitList = mapLoader->getUnitList();
    depot = mapLoader->getDepot();
    convoyList = mapLoader->getConvoyList();
	zombieList = mapLoader->getZombieList();
    unitConvoys.insert(unitConvoys.end(), unitList.begin(), unitList.end());
    unitConvoys.insert(unitConvoys.end(), convoyList.begin(), convoyList.end());
    vector<GameObject*> allObjects;
	allObjects.insert(allObjects.end(), unitList.begin(), unitList.end());
	allObjects.insert(allObjects.end(), convoyList.begin(), convoyList.end());
	allObjects.insert(allObjects.end(), zombieList.begin(), zombieList.end());
	allObjects.push_back(depot);

    time = new dayCycle();
    UI = new levelUI(renderer, "art/UI/level/Level.png", this, time);

    selector = new SelectedHandler(allObjects, depot, UI);
}

LevelManager::~LevelManager()
{
}

//On closing the game saves what is needed to be saved
void LevelManager::saveOnExit()
{
	MapSaver saver("maps/test.xml");
    unitConvoys.erase(
        remove_if(unitConvoys.begin(), unitConvoys.end(),
            [](const GameObject* ptr) { return ptr == nullptr; }),
        unitConvoys.end()
    );
    zombieList.erase(
        remove_if(zombieList.begin(), zombieList.end(),
            [](const ZombieObj* ptr) { return ptr == nullptr; }),
        zombieList.end()
    );
    saver.saveFile(unitConvoys, depot, zombieList);
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
    for (auto& unit : unitConvoys) {
        if (unit) {
            unit->Update();
            if (unit->getHealth() <= 0) {
                delete unit;
                unit = nullptr;
            }
            else if (unit->getHovering()) {
                hoveredUnit = unit;
            }
        }
    }
    unitConvoys.erase(
        remove_if(unitConvoys.begin(), unitConvoys.end(),
            [](const GameObject* ptr) { return ptr == nullptr; }),
        unitConvoys.end()
    );
    //selector->setAllObjects(unitConvoys);
    for (auto& zombie : zombieList) { 
        if (zombie) {
            zombie->getComponent<nearestComponent>()->setnearbyUnits(unitConvoys);
            zombie->Update();
            if (zombie->getHealth() <= 0) {
                delete zombie;
                zombie = nullptr;
            }
        }
    }
    camera.update();
    if (hoveredUnit) {
        UI->renderResourceHover();
        hoveredUnit->renderHover(renderer);
    }
    UI->render();
    UI->renderTime();
    depot->renderResources(renderer);
}

void LevelManager::addUnitConvoy(GameObject* unitConvoy) {
    unitConvoys.emplace_back(unitConvoy);
    selector->setAllObjects(unitConvoys);
}


//Handles zoom change
void LevelManager::zoomChange(SDL_Event event) {
    //mouse position before zoom
    float mxb = 0.0f;
    float myb = 0.0f;
    getMapScaledMousePos(&mxb, &myb);
    zoom += event.wheel.y * 0.1; // zoom in or out based on mouse wheel scroll
    clampZoom();
    SDL_SetRenderLogicalPresentation(renderer, ResolutionWidth / zoom, ResolutionHeight / zoom, SDL_LOGICAL_PRESENTATION_LETTERBOX);
    //SDL_SetRenderScale(renderer, zoom, zoom);
    //mouse position after zoom
    float mxa = 0.0f;
    float mya = 0.0f;
    getMapScaledMousePos(&mxa, &mya);
    camera.dimen.x += mxb - mxa;
    camera.dimen.y += myb - mya;
    camera.dimen.w = ResolutionWidth / zoom;
    camera.dimen.h = ResolutionHeight / zoom;
    //Update camera draw scale
    camera.xScale = camera.dimen.w / DESIGN_SCALE_X;
    camera.yScale = camera.dimen.h / DESIGN_SCALE_Y;
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