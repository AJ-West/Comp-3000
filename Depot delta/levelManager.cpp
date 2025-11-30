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
			selector->checkClick();
        }
        else if (event.button.button == SDL_BUTTON_RIGHT) {
            selector->rightClick();
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
        //SDL_FRect unitRes = { 0, 0 + screenHeight - 100, camera.dimen.w, 100 };
        //SDL_SetRenderDrawColor(renderer, 139, 69, 19, 255); // Saddle brown for resource bar background
        //SDL_RenderFillRect(renderer, &unitRes);
        UI->renderResourceHover();
        hoveredUnit->renderHover(renderer);
    }
    UI->render();
    UI->renderTime();
    depot->renderResources(renderer);    
}