#include "levelManager.h"
LevelManager::LevelManager(SDL_Renderer* SDL_Renderer) : renderer(SDL_Renderer)
{
    mapLoader = new MapLoader("maps/test.xml", renderer);
    unitList = mapLoader->getUnitList();
    depot = mapLoader->getDepot();
    convoyList = mapLoader->getConvoyList();
	vector<GameObject*> allObjects;
	allObjects.insert(allObjects.end(), unitList.begin(), unitList.end());
	allObjects.insert(allObjects.end(), convoyList.begin(), convoyList.end());
	selector = new SelectedHandler(allObjects);
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