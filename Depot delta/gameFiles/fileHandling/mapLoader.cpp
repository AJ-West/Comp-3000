#include "gameFiles/fileHandling/mapLoader.h"
#include "gameFiles/useThroughout/stats/unitStats.h"
#include "gameFiles/useThroughout/stats/convoyStats.h"

MapLoader::MapLoader(const char* filename, SDL_Renderer* renderer)
{
    XMLDocument doc;
    doc.LoadFile(filename);
    XMLElement* root = doc.RootElement();
    XMLElement* layer = root->FirstChildElement("layers");
    while (layer) {
        string layerName = string(layer->FirstChildElement("name")->GetText());
        if (layerName == "Tilemap") {
            loadTilemap(layer);
        }
        else if (layerName == "Level Details") {
            loadTime(layer->FirstChildElement("time"));
        }
        else if (layerName == "Swarms") {
            loadSwarms(layer);
        }
        else if (layerName == "Entities") {
            loadEntities(layer);
        }
        layer = layer->NextSiblingElement("layers");

    }

    tilemapTexture = loadTexture("draftArt/map.png");

    //Sets the camera to focus on the center of the map (where the depot is)
    camera.dimen.x = worldWidth / 2 - camera.dimen.w / 2;
    camera.dimen.y = worldHeight / 2 - camera.dimen.h / 2;
}

MapLoader::~MapLoader()
{
}

void MapLoader::readTechFile() {
    XMLDocument doc;
    doc.LoadFile("techTree/currentTree.xml");
    XMLElement* root = doc.RootElement();
    XMLElement* layer = root->FirstChildElement("layers");
    while (layer) {
        string layerName = string(layer->FirstChildElement("name")->GetText());
        if (layerName == "depotUpgrades") {
            loadDepotUpgrades(layer);
        }
        else if (layerName == "unitUpgrades") {
            loadUnitUpgrades(layer);
        }
        else if (layerName == "convoyUpgrades") {
            loadConvoyUpgrades(layer);
        }
        layer = layer->NextSiblingElement("layers");
    }
}

void MapLoader::loadDepotUpgrades(XMLElement* layer) {
    XMLElement* entity = layer->FirstChildElement("entities");
    while (entity) {
        string name = string(entity->FirstChildElement("name")->GetText());
        string keyName = string(entity->FirstChildElement("keyName")->GetText());
        int type = atoi(entity->FirstChildElement("type")->GetText());

        switch (type) {
        case modifier: {
            int boughtAmount = atoi(entity->FirstChildElement("boughtAmount")->GetText());
            if (boughtAmount != 0) {
                int modifyValue = atoi(entity->FirstChildElement("modifyValue")->GetText());
                depotTechVal[keyName] *= boughtAmount * modifyValue;
            }
            break;
        }
        case unlock: // not yet implemented in tech tree
            break;
        }
    }
}

void MapLoader::loadUnitUpgrades(XMLElement* layer) {
    XMLElement* entity = layer->FirstChildElement("entities");
    while (entity) {
        string name = string(entity->FirstChildElement("name")->GetText());
        string keyName = string(entity->FirstChildElement("keyName")->GetText());
        int type = atoi(entity->FirstChildElement("type")->GetText());

        switch (type) {
        case modifier: {
            int boughtAmount = atoi(entity->FirstChildElement("boughtAmount")->GetText());
            if (boughtAmount != 0) {
                int modifyValue = atoi(entity->FirstChildElement("modifyValue")->GetText());
                unitTechVal[keyName] *= boughtAmount * modifyValue;
            }
            break;
        }
        case unlock: // not yet implemented in tech tree
            break;
        }
    }
}

void MapLoader::loadConvoyUpgrades(XMLElement* layer) {
    XMLElement* entity = layer->FirstChildElement("entities");
    while (entity) {
        string name = string(entity->FirstChildElement("name")->GetText());
        string keyName = string(entity->FirstChildElement("keyName")->GetText());
        int type = atoi(entity->FirstChildElement("type")->GetText());

        switch (type) {
        case modifier: {
            int boughtAmount = atoi(entity->FirstChildElement("boughtAmount")->GetText());
            if (boughtAmount != 0) {
                int modifyValue = atoi(entity->FirstChildElement("modifyValue")->GetText());
                convoyTechVal[keyName] *= boughtAmount * modifyValue;
            }
            break;
        }
        case unlock: // not yet implemented in tech tree
            break;
        }
    }
}

void MapLoader::loadTilemap(XMLElement* layer)
{
    worldWidth = atoi(layer->FirstChildElement("width")->GetText());
    worldHeight = atoi(layer->FirstChildElement("height")->GetText());
    WORLD_TILE_SIZE = atoi(layer->FirstChildElement("gridCellSize")->GetText());
    WORLD_TILE_COLS = worldWidth / WORLD_TILE_SIZE;
    WORLD_TILE_ROWS = worldHeight / WORLD_TILE_SIZE;
	grid.resize(WORLD_TILE_ROWS, vector<Tile>(WORLD_TILE_COLS));
	for (int i = 0; i < WORLD_TILE_ROWS; i++) {
        for (int j = 0; j < WORLD_TILE_COLS; j++) {
            grid[i][j].walkable = true;
        }
	}
}

void MapLoader::loadTime(XMLElement* layer) {
    time[day] = atoi(layer->FirstChildElement("day")->GetText());
    time[hour] = atoi(layer->FirstChildElement("hour")->GetText());
    time[minute] = atoi(layer->FirstChildElement("minute")->GetText());
    time[second] = atoi(layer->FirstChildElement("second")->GetText());
}

void MapLoader::loadSwarms(XMLElement* layer) {
    if (atoi(layer->FirstChildElement("swarmCount")->GetText()) != 0) {
        int count = 1;
        string name = "swarm" + to_string(count);
        XMLElement* entity = layer->FirstChildElement(name.c_str());
        while (entity) {
            vector<int> quantities;
            quantities.emplace_back(atoi(entity->FirstChildElement("basicQuantity")->GetText()));
            quantities.emplace_back(atoi(entity->FirstChildElement("bruteQuantity")->GetText()));
            quantities.emplace_back(atoi(entity->FirstChildElement("quickQuantity")->GetText()));
            swarmQuantity.emplace_back(quantities);
            swarmDirection.emplace_back(atoi(entity->FirstChildElement("direction")->GetText()));
            XMLElement* time = entity = entity->FirstChildElement("time");
            vector<int> sTime{0, 0, 0, 0 };
            sTime[second] = atoi(time->FirstChildElement("second")->GetText());
            sTime[minute] = atoi(time->FirstChildElement("minute")->GetText());
            sTime[hour] = atoi(time->FirstChildElement("hour")->GetText());
            sTime[day] = atoi(time->FirstChildElement("day")->GetText());
            swarmTimes.emplace_back(sTime);

            count++;
            name = "swarm" + to_string(count);
            entity = entity->NextSiblingElement(name.c_str());
        }
    }
}

void MapLoader::loadEntities(XMLElement* layer)
{
    XMLElement* entity = layer->FirstChildElement("entities");
	while (entity) {
		string name = string(entity->FirstChildElement("name")->GetText());
		if (name == "Basic unit") {
			loadUnit(entity);
		}
        else if (name == "depot") {
			loadDepot(entity);
        }
        else if (name == "Basic Convoy") {
            loadConvoy(entity);
        }
        else if (name == "Basic Zombie") {
            loadZombie(entity);
        }
        else if (name == "Building") {
            loadBuilding(entity);
        }
		entity = entity->NextSiblingElement("entities");
	}
    loadAllTransfer(layer);
    for (auto& unit : *unitConvoyList) {
        if (unit->getUnitOrConvoy() == UNIT) {
            unit->getComponent<attackComponent>()->setPotentialTargets(zombieList);
        }
    }
	for (auto& zombie : *zombieList) {
		zombie->getComponent<nearestComponent>()->setnearbyUnits(unitConvoyList);
		zombie->getComponent<nearestComponent>()->setnearbyBuildings(buildingList);
        zombie->getComponent<nearestComponent>()->setDepot(depot);
	}

    loadTechTree();
}

void MapLoader::loadUnit(XMLElement* entity)
{
    int x = atoi(entity->FirstChildElement("x")->GetText());
    int y = atoi(entity->FirstChildElement("y")->GetText());
    int health = atoi(entity->FirstChildElement("health")->GetText());
    int id = atoi(entity->FirstChildElement("id")->GetText());
    int width = unitStats.width; // number of tiles
    int height = unitStats.height;
    auto unit = make_shared<UnitObj>(x, y, width, height, health, id);
    addUnitComponents(unit.get(), entity);
    if (entity->FirstChildElement("target_x")) {
        unit->setTarget(atoi(entity->FirstChildElement("target_x")->GetText()), atoi(entity->FirstChildElement("target_y")->GetText()));
    }
    unitConvoyList->emplace_back(unit);
}

void MapLoader::addUnitComponents(UnitObj* unit, XMLElement* entity) {
    if (entity->FirstChildElement("Resources")) {
		XMLElement* resources = entity->FirstChildElement("Resources");
        unitStats.rCount[PERSONNEL] = atoi(resources->FirstChildElement("Personnel")->GetText());
        unitStats.rCount[AMMUNITION] = atoi(resources->FirstChildElement("Ammunition")->GetText());
        unitStats.rCount[DOS] = atoi(resources->FirstChildElement("DoS")->GetText());
        unitStats.rCount[FUEL] = atoi(resources->FirstChildElement("Fuel")->GetText());
        unitStats.rCount[SCRAP] = atoi(resources->FirstChildElement("Scrap")->GetText());
    }
    unitStats.addComponents(unit);
}

void MapLoader::loadConvoy(XMLElement* entity)
{
    int x = atoi(entity->FirstChildElement("x")->GetText());
    int y = atoi(entity->FirstChildElement("y")->GetText());
    int health = atoi(entity->FirstChildElement("health")->GetText());
    int id = atoi(entity->FirstChildElement("id")->GetText());
    int width = convoyStats.width; // number of tiles
    int height = convoyStats.height;
    ConvoyObj* convoy = new ConvoyObj(x, y, width, height, health, id);
    addConvoyComponents(convoy, entity);
    if (entity->FirstChildElement("target_x")) {
        convoy->setTarget(atoi(entity->FirstChildElement("target_x")->GetText()), atoi(entity->FirstChildElement("target_y")->GetText()));
    }
    unitConvoyList->emplace_back(convoy);
}

void MapLoader::addConvoyComponents(ConvoyObj* convoy, XMLElement* entity) {
    if (entity->FirstChildElement("Resources")) {
        XMLElement* resources = entity->FirstChildElement("Resources");
        convoyStats.rCount[PERSONNEL] = atoi(resources->FirstChildElement("Personnel")->GetText());
        convoyStats.rCount[AMMUNITION] = atoi(resources->FirstChildElement("Ammunition")->GetText());
        convoyStats.rCount[DOS] = atoi(resources->FirstChildElement("DoS")->GetText());
        convoyStats.rCount[FUEL] = atoi(resources->FirstChildElement("Fuel")->GetText());
        convoyStats.rCount[SCRAP] = atoi(resources->FirstChildElement("Scrap")->GetText());
    }
    convoyStats.addComponents(convoy);
}

void MapLoader::loadZombie(XMLElement* entity)
{
    int x = atoi(entity->FirstChildElement("x")->GetText());
    int y = atoi(entity->FirstChildElement("y")->GetText());
    int health = atoi(entity->FirstChildElement("health")->GetText());
    int id = atoi(entity->FirstChildElement("id")->GetText());
    int width = 4; // number of tiles
    int height = 4;
    int sight = 500.0f; // default zombie sight
    if (entity->FirstChildElement("sight")) {
        sight = atoi(entity->FirstChildElement("sight")->GetText());
    }
    ZombieObj* zombie;
    int type = atoi(entity->FirstChildElement("type")->GetText());
    switch (type) {
    case BRUTE: {
        bruteZombieStats stats;
        zombie = new ZombieObj(x, y, stats.size, stats.size, stats.maxHealth, id, type);
        stats.addComponents(zombie, sight); // covers size of map
        break;
    }
    case QUICK: {
        quickZombieStats stats;
        zombie = new ZombieObj(x, y, stats.size, stats.size, stats.maxHealth, id, type);
        stats.addComponents(zombie, sight); // covers size of map
        break;
    }
    default: {
        zombieStats stats;
        zombie = new ZombieObj(x, y, stats.size, stats.size, stats.maxHealth, id, type);
        stats.addComponents(zombie, sight); // covers size of map
    }
    }
    if (entity->FirstChildElement("target_x")) {
        zombie->setTarget(atoi(entity->FirstChildElement("target_x")->GetText()), atoi(entity->FirstChildElement("target_y")->GetText()));
    }
    zombieList->emplace_back(zombie);
}

void MapLoader::loadDepot(XMLElement* entity)
{
    int x = atoi(entity->FirstChildElement("x")->GetText());
    int y = atoi(entity->FirstChildElement("y")->GetText());
    int health = atoi(entity->FirstChildElement("health")->GetText());
    int width = 16; // number of tiles 
    int height = 16;
    depot = new DepotObj(x, y, width, height, health);
	addDepotComponents(depot, entity);
}

void MapLoader::addDepotComponents(DepotObj* depot, XMLElement* entity) {
    depot->AddComponent(make_shared<renderComponent>(depot, renderer, "draftArt/depot.png"));
    depot->AddComponent(make_shared<buttonComponent>(depot));
    depot->AddComponent(make_shared<movementComponent>(depot, 50));
    vector<int> max = { 1000, 1000, 1000, 1000, 1000 };
    vector<int> count = { 500, 500, 500, 500, 500 };
    if (entity->FirstChildElement("Resources")) {
        XMLElement* resources = entity->FirstChildElement("Resources");
        count[PERSONNEL] = atoi(resources->FirstChildElement("Personnel")->GetText());
        count[AMMUNITION] = atoi(resources->FirstChildElement("Ammunition")->GetText());
        count[DOS] = atoi(resources->FirstChildElement("DoS")->GetText());
        count[FUEL] = atoi(resources->FirstChildElement("Fuel")->GetText());
        count[SCRAP] = atoi(resources->FirstChildElement("Scrap")->GetText());
    }
    depot->AddComponent(make_shared<resourceComponent>(depot, max, count, loadResourceTextures()));
}

void MapLoader::loadBuilding(XMLElement* entity) {
    int x = atoi(entity->FirstChildElement("x")->GetText());
    int y = atoi(entity->FirstChildElement("y")->GetText());
    int health = atoi(entity->FirstChildElement("health")->GetText());
    int width = 8; // number of tiles 
    int height = 8;
    bool alive = atoi(entity->FirstChildElement("alive")->GetText()) != 0;
    int type = atoi(entity->FirstChildElement("type")->GetText());
    BuildingObj* building = new BuildingObj(x, y, width, height, health, alive, type, atoi(entity->FirstChildElement("id")->GetText()));
    addBuildingComponents(building, entity);
    buildingList->emplace_back(building);
}

void MapLoader::addBuildingComponents(BuildingObj* building, XMLElement* entity) {
    vector<int> max = { 100, 100, 100, 100, 100 };
    vector<int> count = { 0, 0, 0, 0, 0 };
    max[atoi(entity->FirstChildElement("type")->GetText())] *= 3;
    if (entity->FirstChildElement("Resources")) {
        XMLElement* resources = entity->FirstChildElement("Resources");
        count[PERSONNEL] = atoi(resources->FirstChildElement("Personnel")->GetText());
        count[AMMUNITION] = atoi(resources->FirstChildElement("Ammunition")->GetText());
        count[DOS] = atoi(resources->FirstChildElement("DoS")->GetText());
        count[FUEL] = atoi(resources->FirstChildElement("Fuel")->GetText());
        count[SCRAP] = atoi(resources->FirstChildElement("Scrap")->GetText());
    }
    building->AddComponent(make_shared<resourceComponent>(building, max, count, loadResourceTextures()));
    building->AddComponent(make_shared<buttonComponent>(building));
    building->AddComponent(make_shared<renderComponent>(building, renderer, entity->FirstChildElement("art")->GetText()));
}

void MapLoader::loadAllTransfer(XMLElement* layer) {
    XMLElement* entity = layer->FirstChildElement("entities");
    while (entity) {
        string name = string(entity->FirstChildElement("name")->GetText());
        if (name == "Basic Convoy") {
            loadTransfer(entity);
        }
        entity = entity->NextSiblingElement("entities");
    }
}

void MapLoader::loadTransfer(XMLElement* entity) {
    for (auto unit : *unitConvoyList) {
        if (entity->FirstChildElement("Transfering")) {
            if (entity->FirstChildElement("Transfering")->GetText() != "- 1") {
                if (XMLElement* resourceTransfer = entity->FirstChildElement("ResourceTransfer")) {
                    vector<int> transferAmount = { 0,0,0,0,0 };
                    transferAmount[PERSONNEL] = atoi(resourceTransfer->FirstChildElement("PersonnelChange")->GetText());
                    transferAmount[AMMUNITION] = atoi(resourceTransfer->FirstChildElement("AmmunitionChange")->GetText());
                    transferAmount[DOS] = atoi(resourceTransfer->FirstChildElement("DoSChange")->GetText());
                    transferAmount[FUEL] = atoi(resourceTransfer->FirstChildElement("FuelChange")->GetText());
                    transferAmount[SCRAP] = atoi(resourceTransfer->FirstChildElement("ScrapChange")->GetText());
                    for (auto convoy : *unitConvoyList) {
                        if (convoy->getID() == atoi(entity->FirstChildElement("Transfering")->GetText())) {
                            unit->getComponent<resourceTransferComponent>()->initiateTransfer(convoy.get(), transferAmount);
                        }
                    }
                }
            }
        }
    }
}

void MapLoader::loadTechTree() {
    XMLDocument doc;
    doc.LoadFile("techTree/currentTree.xml");
    XMLElement* root = doc.RootElement();
    XMLElement* layer = root->FirstChildElement("layers");
    while (layer) {
        string layerName = string(layer->FirstChildElement("name")->GetText());
        if (layerName == "depotUpgrades") {
            loadDepotTech(layer);
        }
        else if (layerName == "unitUpgrades") {
            loadUnitTech(layer);
        }
        else if (layerName == "convoyUpgrades") {
            loadConvoyTech(layer);
        }
        layer = layer->NextSiblingElement("layers");
    }
}

void MapLoader::loadDepotTech(XMLElement* layer) {
    XMLElement* entity = layer->FirstChildElement("entities");
    while (entity) {
        string keyName = entity->FirstChildElement("keyName")->GetText();
        depotTechVal[keyName] *= pow(atof(entity->FirstChildElement("keyName")->GetText()), atoi(entity->FirstChildElement("boughtAmount")->GetText()));
        depot->updateStats(keyName);
        entity = entity->NextSiblingElement("entities");
    }
}

void MapLoader::loadUnitTech(XMLElement* layer) {
    XMLElement* entity = layer->FirstChildElement("entities");
    while (entity) {
        string keyName = entity->FirstChildElement("keyName")->GetText();
        unitTechVal[keyName] *= pow(atof(entity->FirstChildElement("modifyValue")->GetText()), atoi(entity->FirstChildElement("boughtAmount")->GetText()));
        for (auto unit : *unitConvoyList) {
            unit->updateStats(keyName, true);
        }
        entity = entity->NextSiblingElement("entities");
    }
}

void MapLoader::loadConvoyTech(XMLElement* layer) {
    XMLElement* entity = layer->FirstChildElement("entities");
    while (entity) {
        string keyName = entity->FirstChildElement("keyName")->GetText();
        convoyTechVal[keyName] *= pow(atof(entity->FirstChildElement("modifyValue")->GetText()), atoi(entity->FirstChildElement("boughtAmount")->GetText()));
        for (auto convoy : *unitConvoyList) {
            convoy->updateStats(keyName, false);
        }
        entity = entity->NextSiblingElement("entities");
    }
}

void MapLoader::renderTileMap(SDL_Renderer* renderer) {
    SDL_FRect destRect{-camera.dimen.x,-camera.dimen.y,worldWidth,worldHeight };
    SDL_RenderTexture(renderer, tilemapTexture, NULL, &destRect);
    //for (auto& row : tilemap) {
      //  for (auto& column : row) {
        //    SDL_RenderTexture(renderer, tilemapTexture, &column, &destRect);
            //SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
			//SDL_RenderRect(renderer, &destRect);
          //  destRect.x += tileWidth;
        //}
		//destRect.x = -camera.dimen.x;
		//destRect.y += tileHeight;
    //}
}