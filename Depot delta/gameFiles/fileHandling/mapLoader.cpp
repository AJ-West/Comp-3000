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
        else if (layerName == "Entities") {
            loadEntities(layer);
        }
        layer = layer->NextSiblingElement("layers");

    }

    //SDL_Surface* surface = IMG_Load("art/environment/tilemap.png");
    SDL_Surface* surface = IMG_Load("draftArt/map.png");
    if (!surface) {
        cerr << "Unable to load image! IMG_Error: " << SDL_GetError() << endl;
        return;
    }
    tilemapTexture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_SetTextureScaleMode(tilemapTexture, SDL_SCALEMODE_PIXELART);
    SDL_DestroySurface(surface); // Free the surface after creating the texture
    if (!tilemapTexture) {
        cerr << "Unable to create texture! SDL_Error: " << SDL_GetError() << endl;
        return;
    }
}

MapLoader::~MapLoader()
{
}

void MapLoader::loadTilemap(XMLElement* layer)
{
    tileWidth = atoi(layer->FirstChildElement("gridCellWidth")->GetText());
    tileHeight = atoi(layer->FirstChildElement("gridCellHeight")->GetText());
    xCells = atoi(layer->FirstChildElement("gridCellsX")->GetText());
    yCells = atoi(layer->FirstChildElement("gridCellsY")->GetText());
	grid.resize(yCells, vector<Tile>(xCells));
    //XMLElement* data = layer->FirstChildElement("data");
	for (int i = 0; i < yCells; i++) {
		vector<SDL_FRect> row;
		/*for (int j = 0; j < xCells; j++) {
			int pos = atoi(data->GetText()); // position in spritesheet
            div_t loc = div(pos, 8); // gettile value as int then conver to location on tilemap spritesheet
			SDL_FRect cell;
			cell.x = loc.rem * tileWidth;
			cell.y = loc.quot * tileHeight;
			cell.w = tileWidth;
			cell.h = tileHeight;
			row.push_back(cell);
            data = data->NextSiblingElement("data");
            if (pos >= 20) { grid[i][j].walkable = true; } // if tile is generic (hardcoded while using basic tilemap)
		}*/
        for (int j = 0; j < xCells; j++) {
            grid[i][j].walkable = true;
        }
		tilemap.push_back(row);
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
        if (typeid(*unit).name() == typeid(UnitObj).name()) { 
            unit->getComponent<attackComponent>()->setPotentialTargets(zombieList);
        }
    }
	for (auto& zombie : *zombieList) {
		zombie->getComponent<nearestComponent>()->setnearbyUnits(unitConvoyList);
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
    ZombieObj* zombie = new ZombieObj(x, y, width, height, health, id);
    addZombieComponents(zombie);
    if (entity->FirstChildElement("target_x")) {
        zombie->setTarget(atoi(entity->FirstChildElement("target_x")->GetText()), atoi(entity->FirstChildElement("target_y")->GetText()));
    }
    zombieList->emplace_back(zombie);
}

void MapLoader::addZombieComponents(ZombieObj* zombie) {
    zombieStats stats;
    stats.addComponents(zombie, 50.0f);
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
    depot->getComponent<resourceComponent>()->setResourceChange(PERSONNEL, 1);
    depot->getComponent<resourceComponent>()->setResourceChange(AMMUNITION, 1);
    depot->getComponent<resourceComponent>()->setResourceChange(DOS, 1);
    depot->getComponent<resourceComponent>()->setResourceChange(FUEL, 1);
    depot->getComponent<resourceComponent>()->setResourceChange(SCRAP, 1);
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