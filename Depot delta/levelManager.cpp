#include "levelManager.h"
LevelManager::LevelManager(SDL_Renderer* SDL_Renderer) : renderer(SDL_Renderer)
{
    mapLoader = new MapLoader("maps/test.xml", renderer);
    unitList = mapLoader->getUnitList();
    depot = mapLoader->getDepot();
    convoyList = mapLoader->getConvoyList();
}

LevelManager::~LevelManager()
{
}

void LevelManager::exit()
{
	MapSaver saver("maps/test.xml");
    saver.saveFile(unitList, depot, convoyList);
}

void LevelManager::handleInput(SDL_Event event)
{
    if (event.type == SDL_EVENT_MOUSE_MOTION) {
        checkHover(event);
            }
    else if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
        if (event.button.button == SDL_BUTTON_LEFT) {
            checkClick();
        }
            }
    else if (event.type == SDL_EVENT_KEY_DOWN) {
        camera.keyDown(event.key.key);
            }
    else if (event.type == SDL_EVENT_KEY_UP) {
        camera.keyUp(event.key.key);
            }
}

void LevelManager::render()
{
	mapLoader->renderTileMap(renderer);
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
    depot->Update();
    camera.update();
    if (hoveredUnit) {
        SDL_FRect unitRes = { 0, 0 + screenHeight - 100, camera.width, 100 };
        SDL_SetRenderDrawColor(renderer, 139, 69, 19, 255); // Saddle brown for resource bar background
        SDL_RenderFillRect(renderer, &unitRes);
        hoveredUnit->renderHover(renderer);
    }
    SDL_FRect depotRes = { 0, 0, camera.width, 40 };
    SDL_SetRenderDrawColor(renderer, 139, 69, 19, 255); // Saddle brown for resource bar background
    SDL_RenderFillRect(renderer, &depotRes);
    depot->renderHover(renderer);
}


void LevelManager::checkHover(SDL_Event event) {
    for (auto& unit : unitList) {
        unit->checkHover(event.motion.x, event.motion.y);
    }
    for (auto& convoy : convoyList) {
        convoy->checkHover(event.motion.x, event.motion.y);
    }
}

void LevelManager::checkClick() {
    UnitObj* selectedUnit = nullptr;
    ConvoyObj* selectedConvoy = nullptr;
    //see if there is a currently selected unit
    for (auto& unit : unitList) {
        if (unit->getSelected()) {
            selectedUnit = unit;
            break;
        }
    }

    //see if there is a currently selected convoy
    for (auto& convoy : convoyList) {
        if (convoy->getSelected()) {
            selectedConvoy = convoy;
            break;
        }
    }

    bool selectedSomething = false;

    //see if a unit has been clicked
    for (auto& unit : unitList) {
        if (unit->getHovering() && !unit->getSelected()) {
            unit->onClick();
            if (selectedUnit) {
                selectedUnit->onClick();
            }
            selectedUnit = unit;
            selectedSomething = true;
            break;
        }
        else if (unit->getHovering() && unit->getSelected()) {
            selectedUnit = nullptr;
            unit->onClick();
            selectedSomething = true;
            break;

        }
    }

    if (!selectedSomething) {
        //see if a convoy has been clicked
        for (auto& convoy : convoyList) {
            if (convoy->getHovering() && !convoy->getSelected()) {
                convoy->onClick();
                if (selectedConvoy) {
                    selectedConvoy->onClick();
                }
                selectedConvoy = convoy;
                selectedSomething = true;
                if (selectedUnit) {
                    selectedUnit->onClick();
                    selectedUnit = nullptr;
                }
                break;
            }
            else if (convoy->getHovering() && convoy->getSelected()) {
                selectedConvoy = nullptr;
                convoy->onClick();
                selectedSomething = true;
                break;

            }
        }
    }
    else if (selectedConvoy) {
        selectedConvoy->onClick();
        selectedConvoy = nullptr;
    }

    if (!selectedSomething && selectedUnit) {
        selectedUnit->clickAway();
    }
    else if (!selectedSomething && selectedConvoy) {
        selectedConvoy->clickAway();
    }
}