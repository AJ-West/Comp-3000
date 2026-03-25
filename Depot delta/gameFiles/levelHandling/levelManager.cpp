#include "gameFiles/levelHandling/levelManager.h"

#include "gameFiles/UI/levelUI.h"
#include "gameFiles/entities/Zombies/zombieSpawner.h"
#include "gameFiles/levelHandling/tutorialManager/tutorialManager.h"

LevelManager::LevelManager(SDL_Renderer* SDL_Renderer) : renderer(SDL_Renderer)
{
    mapLoader = new MapLoader("maps/demo.xml", renderer);
    unitConvoys = mapLoader->getUnitConvoyList();
    buildingList = mapLoader->getBuildingList();
    depot = shared_ptr<DepotObj>(mapLoader->getDepot());
    zombieList = mapLoader->getZombieList();
	allObjects->insert(allObjects->end(), unitConvoys->begin(), unitConvoys->end());
	allObjects->insert(allObjects->end(), buildingList->begin(), buildingList->end());
	allObjects->emplace_back(depot);

    winConditions = mapLoader->getWinCons();
    if (winConditions[WINCON::resourceCount]) { winResources = mapLoader->getWinResources(); }

    for (auto zombie : *zombieList) { // needs seperate for each due so can set priorities in nearest component
        zombie->getComponent<nearestComponent>()->setnearbyUnits(getUnitConvoys());
        zombie->getComponent<nearestComponent>()->setnearbyBuildings(buildingList);
        zombie->getComponent<nearestComponent>()->setDepot(getDepot().get());
    }

    time = new dayCycle(5, mapLoader->getTime(), mapLoader->getSwarmTimes(), mapLoader->getSwarmQuantity(), mapLoader->getSwarmDirection());

    UI = new levelUI(renderer, "art/UI/level/Level.png", this, time);

    spawner = new ZombieSpawner(this);

    handler = new HandleSelected(UI);

    tManager = new TutorialManager();
    if (level == 0) {
        tManager->changeScene(new DepotPointScreen(this));
        paused = true;
        tutorial = true;
    }
}

LevelManager::~LevelManager()
{
}

bool LevelManager::checkWin() {
    bool win = true;
    if (winConditions[WINCON::surviveWaves]) {
        if (time->getSwarmsLeft()) { return false; }
        else { cout << "all swarms defeated" << '\n'; }
    }
    if (winConditions[WINCON::clearZombies]) {
        if (zombieList->size() != 0) { return false; }
        else { cout << "no zombies left" << '\n'; }
    }
    if (winConditions[WINCON::resourceCount]) {
        int i = 0;
        vector<int> depotResources = depot->getComponent<resourceComponent>()->getAllResourceCount();
        for (auto resource : winResources) {
            if (resource < depotResources[i]) { return false; }
            i++;
        }
        cout << "resource goal met" << '\n'; 
    }
    return win;
}

//On closing the game saves what is needed to be saved
void LevelManager::saveOnExit()
{
	MapSaver saver("maps/demo.xml");
    saver.saveFile(allObjects, *zombieList, time);
}

// Handles user input
void LevelManager::handleInput(SDL_Event event)
{
    if (tutorial) {
        if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
            bool nextTutorial = tManager->getNextTutorial();
            if (!nextTutorial && !UI->getBox() && !UI->getTree()) {
                tutorial = false;
                paused = false;
            }
            else if (!nextTutorial) {
                tutorial = false;
            }
        }
    }
    else {
        handler->handleInput(event, this);
    }
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
    checkTutorial(UNITNOAMMO);

    time->update();
    mapLoader->renderTileMap(renderer);
    //if (spawner->checkIfSpawn()) {
      //  for (auto& unit : *unitConvoys) {
        //    if (unit) {
          //      if (typeid(*unit).name() == typeid(UnitObj).name()) {
            //        unit->getComponent<attackComponent>()->setPotentialTargets(zombieList);
              //  }
            //}
        //}
    //}

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
        if (unit && unit->getAlive()) {
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

    if (spawningSwarm) {
        if (swarmLeft[swarmTypeIndex] > 0 && frameStart - lastSpawnTime >= spawnDelay) {
            spawnZombie(swarmTypeIndex);
            swarmLeft[swarmTypeIndex]--;
            lastSpawnTime = frameStart;
        }
        else if(swarmLeft[swarmTypeIndex] == 0){
            swarmTypeIndex++;
            if (swarmTypeIndex >= swarmLeft.size()) {
                spawningSwarm = false;
            }
        }
    }

    for (auto& zombie : *zombieList) {
        if (zombie && zombie->getAlive()) {
            zombie->Update();
        }
    }   

    camera.update();
    if (hoveredUnit) {
        UI->renderResourceHover();
        hoveredUnit->renderHover(renderer);
    }
    UI->render();
    depot->renderResources(renderer);

    if (frameCount % 30 == 0) {
        removeDeadFromLists();
        checkWin();
    }
}

void LevelManager::removeDeadFromLists() {
    unitConvoys->erase(
        remove_if(unitConvoys->begin(), unitConvoys->end(),
            [](shared_ptr<HumanObj> ptr) { return ptr->getHealth() <= 0; }),
        unitConvoys->end()
    );

    zombieList->erase(
        remove_if(zombieList->begin(), zombieList->end(),
            [](shared_ptr<ZombieObj> ptr) { return ptr->getHealth() <= 0; }),
        zombieList->end()
    );
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
    depot->renderResources(renderer);
    tManager->render(renderer);
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
        if (forUnit && unit->getUnitOrConvoy() == UNIT) {
            unit->updateStats(keyName, forUnit);
        }
        else if (forUnit && unit->getUnitOrConvoy() == CONVOY) {
            unit->updateStats(keyName, forUnit);
        }
    }
}

void LevelManager::spawnSwarm(vector<int> quantity, int direction) {
    spawningSwarm = true;
    swarmLeft = quantity;
    swarmDirection = direction;
    swarmTypeIndex = 0;

    switch (swarmDirection) {
    case NORTH:
        swarmPos = { static_cast<float>(worldWidth) / 2.0f, 0 };
        break;
    case NORTHEAST:
        swarmPos = { static_cast<float>(worldWidth), 0 };
        break;
    case EAST:
        swarmPos = { static_cast<float>(worldWidth), static_cast<float>(worldHeight) / 2.0f };
        break;
    case SOUTHEAST:
        swarmPos = { static_cast<float>(worldWidth), static_cast<float>(worldHeight) };
        break;
    case SOUTH:
        swarmPos = { static_cast<float>(worldWidth) / 2.0f, static_cast<float>(worldHeight) };
        break;
    case SOUTHWEST:
        swarmPos = { 0, static_cast<float>(worldHeight) };
        break;
    case WEST:
        swarmPos = { 0, static_cast<float>(worldHeight) / 2 };
        break;
    case NORTHWEST:
        swarmPos = { 0, 0 };
        break;
    }
}

void LevelManager::spawnZombie(int type) {
    // check for where is valid to spawn will later be done by areas 'claimed' by the player (where their units currently are)
    Vec2 depotPos = depot->getPos();
    ZombieObj* zombie;
    switch (type) {
    case BRUTE: {
        bruteZombieStats stats;
        zombie = new ZombieObj(swarmPos.x, swarmPos.y, stats.size, stats.size, stats.maxHealth, getNextID(), type);
        stats.addComponents(zombie, sqrt(worldWidth * worldWidth + worldHeight * worldHeight)); // covers size of map
        break;
    }
    case QUICK: {
        quickZombieStats stats;
        zombie = new ZombieObj(swarmPos.x, swarmPos.y, stats.size, stats.size, stats.maxHealth, getNextID(), type);
        stats.addComponents(zombie, sqrt(worldWidth * worldWidth + worldHeight * worldHeight)); // covers size of map
        break;
    }
    default: {
        zombieStats stats;
        zombie = new ZombieObj(swarmPos.x, swarmPos.y, stats.size, stats.size, stats.maxHealth, getNextID(), type);
        stats.addComponents(zombie, sqrt(worldWidth * worldWidth + worldHeight * worldHeight)); // covers size of map
    }
    }  
    zombie->getComponent<nearestComponent>()->setnearbyUnits(getUnitConvoys());
    zombie->getComponent<nearestComponent>()->setnearbyBuildings(buildingList);
    zombie->getComponent<nearestComponent>()->setDepot(getDepot().get());
    addZombie(zombie);
}

int LevelManager::getNextID() {
    int nextID = 1;
    for (auto obj : *allObjects) {
        if (obj->getID() <= nextID) {
            nextID = obj->getID() + 1;
        }
    }
    for (auto zomb : *zombieList) {
        if (zomb->getID() <= nextID) {
            nextID = zomb->getID() + 1;
        }
    }
    return nextID;
}

void LevelManager::checkTutorial(int tutorialValue) {
    if (level != 0) { return; }
    if (tutorialDone[tutorialValue]) { return; }
    switch (tutorialValue) {
    case tutorialStages::DEPOTPOINT:
        tManager->changeScene(new DepotPointScreen(this));
        paused = true;
        tutorial = true;
        tutorialDone[tutorialValue] = true;
        break;
    case tutorialStages::UNITCREATESCREEN:
        tManager->changeScene(new UnitCreateScreen(this));
        paused = true;
        tutorial = true;
        tutorialDone[tutorialValue] = true;
        break;
    case tutorialStages::RESOURCEPOINT:
        tManager->changeScene(new ResourcePointScreen(this));
        paused = true;
        tutorial = true;
        tutorialDone[tutorialValue] = true;
        break;
    case tutorialStages::SELECTINGUNIT:
        tManager->changeScene(new SelectingUnitScreen(this));
        paused = true;
        tutorial = true;
        tutorialDone[tutorialValue] = true;
        break;
    case tutorialStages::UNITNOAMMO: {
        vector<shared_ptr<HumanObj>> units = *getUnitConvoys();
        if (units.size() > 0) {
            if (units[0]->getComponent<resourceComponent>()->getResourcesCount(AMMUNITION) <= 0) {
                tManager->changeScene(new UnitNoAmmoScreen(this));
                paused = true;
                tutorial = true;
                tutorialDone[tutorialValue] = true;
            }
        }
        break;
        }
    case tutorialStages::CREATECONVOY:
        tManager->changeScene(new CreateConvoyScreen(this));
        paused = true;
        tutorial = true;
        tutorialDone[tutorialValue] = true;
        break;
    case tutorialStages::SELECTINGCONVOY: {
        bool foundConvoy = false;
        int index = 0;
        for (auto& unit : *unitConvoys) {
            if (typeid(*unit).name() == typeid(ConvoyObj).name()) {
                foundConvoy = true;
                break;
            }
            index++;
        }
        if (foundConvoy) {
            tManager->changeScene(new SelectingConvoyScreen(this, index));
            paused = true;
            tutorial = true;
            tutorialDone[tutorialValue] = true;
        }
        break;
    }
    case tutorialStages::RESOURCETRANSFER:
        tManager->changeScene(new ResourceTransferScreen(this));
        paused = true;
        tutorial = true;
        tutorialDone[tutorialValue] = true;
        break;
    case tutorialStages::BUILDINGS:
        tManager->changeScene(new BuildingsScreen(this));
        paused = true;
        tutorial = true;
        tutorialDone[tutorialValue] = true;
        break;
    case tutorialStages::WINCONDITION:
        tManager->changeScene(new BuildingsScreen(this));
        paused = true;
        tutorial = true;
        tutorialDone[tutorialValue] = true;
        break;
    }
}