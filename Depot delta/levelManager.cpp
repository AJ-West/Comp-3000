#include "levelManager.h"
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
	selector = new SelectedHandler(allObjects);

    time = new dayCycle();
    UI = new levelUI(renderer, "art/UI/level/Level.png", time);
    UI->createTransferBox();
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
		selector->checkHover(event);
    }
    else if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
        if (event.button.button == SDL_BUTTON_LEFT) {
            if (!textInput) {
                if (UI->checkClickInput()) {
                    textInput = true;
                    return;
                }
                textInput = false;
                selector->checkClick();
            }
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