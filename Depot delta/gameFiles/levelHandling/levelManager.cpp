#include "gameFiles/levelHandling/levelManager.h"

#include "gameFiles/UI/levelUI.h"
#include "gameFiles/entities/Zombies/zombieSpawner.h"

LevelManager::LevelManager(SDL_Renderer* SDL_Renderer) : renderer(SDL_Renderer)
{
    mapLoader = new MapLoader("maps/test.xml", renderer);
    unitConvoys = mapLoader->getUnitConvoyList();
    buildingList = mapLoader->getBuildingList();
    depot = shared_ptr<DepotObj>(mapLoader->getDepot());
    zombieList = mapLoader->getZombieList();
	allObjects->insert(allObjects->end(), unitConvoys->begin(), unitConvoys->end());
	allObjects->insert(allObjects->end(), buildingList->begin(), buildingList->end());
	allObjects->emplace_back(depot);

    time = new dayCycle();
    UI = new levelUI(renderer, "art/UI/level/Level.png", this, time);

    spawner = new ZombieSpawner(this);

    handler = new HandleSelected(UI);
}

LevelManager::~LevelManager()
{
}

//On closing the game saves what is needed to be saved
void LevelManager::saveOnExit()
{
	MapSaver saver("maps/test.xml");
    saver.saveFile(allObjects, *zombieList);
}

// Handles user input
void LevelManager::handleInput(SDL_Event event)
{
    handler->handleInput(event, this);
}

//Renders the screen
void LevelManager::render()
{
    if (!paused) {
        unpausedRender();
    }
    else if (UI->getTree()) {
        UI->render();
    }
    else {
        pausedRender();
    }
}

void LevelManager::unpausedRender()
{
    time->update();
    mapLoader->renderTileMap(renderer);
    if (spawner->checkIfSpawn()) {
        for (auto& unit : *unitConvoys) {
            if (unit) {
                if (typeid(*unit).name() == typeid(UnitObj).name()) {
                    unit->getComponent<attackComponent>()->setPotentialTargets(zombieList);
                }
            }
        }
    }

    depot->Update();
    if (!depot->getAlive()) {
        depotAlive = false;
        return;
    }
    hoveredUnit = nullptr;

    for (auto& building : *buildingList) {
        if (building) {
            building->Update();
            if (building->getHovering()) {
                hoveredUnit = building;
            }
        }
    }

    for (auto& unit : *unitConvoys) {
        if (unit) {
            unit->Update();
            if (unit->getHealth() <= 0) {
                if (unit->getSelected()) {
                    handler->setStateEnum(INT_MAX);
                    handler->decideState(this);
                }
            }
            else if (unit->getHovering()) {
                hoveredUnit = unit;
            }
        }
    }

    unitConvoys->erase(
        remove_if(unitConvoys->begin(), unitConvoys->end(),
            [](shared_ptr<HumanObj> ptr) { return ptr->getHealth() <= 0; }),
        unitConvoys->end()
    );

    for (auto& zombie : *zombieList) {
        if (zombie) {
            zombie->updateTargets(unitConvoys);
            zombie->Update();
        }
    }

    zombieList->erase(
        remove_if(zombieList->begin(), zombieList->end(),
            [](shared_ptr<ZombieObj> ptr) { return ptr->getHealth() <= 0; }),
        zombieList->end()
    );

    camera.update();
    if (hoveredUnit) {
        UI->renderResourceHover();
        hoveredUnit->renderHover(renderer);
    }
    UI->render();
    UI->renderTime();
    depot->renderResources(renderer);
}

void LevelManager::pausedRender()
{    
    mapLoader->renderTileMap(renderer);
    for (auto& obj : *allObjects) {
        obj->getComponent<renderComponent>()->update(obj.get());
    }
    for (auto& zombie : *zombieList) {
        if (zombie) {
            zombie->getComponent<renderComponent>()->update(zombie.get());
        }
    }
    UI->render();
    UI->renderTime();
    depot->renderResources(renderer);
}

void LevelManager::addUnitConvoy(HumanObj* unitConvoy) {
    unitConvoys->emplace_back(unitConvoy);
    allObjects->emplace_back(unitConvoy);
}

void LevelManager::addZombie(ZombieObj* zombie) {
    zombieList->emplace_back(zombie);
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
    else if (zoom < 0.25f) {
        zoom = 0.25f;
    }
}

void LevelManager::updateStats(string keyName, bool forUnit) {
    for (auto unit : *unitConvoys) {
        unit->updateStats(keyName, forUnit);
    }
}
