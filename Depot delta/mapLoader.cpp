#include "mapLoader.h"


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

    SDL_Surface* surface = IMG_Load("art/environment/tilemap.png");
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
    XMLElement* data = layer->FirstChildElement("data");
	for (int i = 0; i < yCells; i++) {
		vector<SDL_FRect> row;
		for (int j = 0; j < xCells; j++) {
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
        if (name == "depot") {
			loadDepot(entity);
        }
        if (name == "Basic Convoy") {
            loadConvoy(entity);
        }
        if (name == "Basic Zombie") {
            loadZombie(entity);
        }
		entity = entity->NextSiblingElement("entities");
	}
	for (auto& zombie : zombieList) {
		zombie->getComponent<nearestComponent>()->setnearbyUnits(unitList);
	}
}

void MapLoader::loadUnit(XMLElement* entity)
{
    int x = atoi(entity->FirstChildElement("x")->GetText());
    int y = atoi(entity->FirstChildElement("y")->GetText());
    int id = atoi(entity->FirstChildElement("id")->GetText());
    int width = 1; // number of tiles
    int height = 1;
    UnitObj* unit = new UnitObj(x, y, width, height, id);
    addUnitComponents(unit, entity);
    if (entity->FirstChildElement("target_x")) {
        unit->setTarget(atoi(entity->FirstChildElement("target_x")->GetText()), atoi(entity->FirstChildElement("target_y")->GetText()));
    }
    unitList.emplace_back(unit);
}

void MapLoader::addUnitComponents(UnitObj* unit, XMLElement* entity) {
    unit->AddComponent(make_shared<renderComponent>(unit, renderer, "draftArt/basicUnit.png"));
    unit->AddComponent(make_shared<buttonComponent>(unit));
    unit->AddComponent(make_shared<movementComponent>(unit, 50));
    vector<int> max = { 100, 100, 100, 100, 100 };
    vector<int> count = { 50, 50, 50, 50, 50 };
    if (entity->FirstChildElement("Resources")) {
		XMLElement* resources = entity->FirstChildElement("Resources");
		count[PERSONNEL] = atoi(resources->FirstChildElement("Personnel")->GetText());
		count[AMMUNITION] = atoi(resources->FirstChildElement("Ammunition")->GetText());
		count[DOS] = atoi(resources->FirstChildElement("DoS")->GetText());
		count[FUEL] = atoi(resources->FirstChildElement("Fuel")->GetText());
		count[SCRAP] = atoi(resources->FirstChildElement("Scrap")->GetText());
    }
    unit->AddComponent(make_shared<resourceComponent>(unit, max, count, loadResourceTextures()));
    unit->AddComponent(make_shared<pathfindingComponent>(unit, grid));
}

void MapLoader::loadConvoy(XMLElement* entity)
{
    int x = atoi(entity->FirstChildElement("x")->GetText());
    int y = atoi(entity->FirstChildElement("y")->GetText());
    int id = atoi(entity->FirstChildElement("id")->GetText());
    int width = 1; // number of tiles
    int height = 1;
    ConvoyObj* convoy = new ConvoyObj(x, y, width, height, id);
    addConvoyComponents(convoy, entity);
    if (entity->FirstChildElement("target_x")) {
        convoy->setTarget(atoi(entity->FirstChildElement("target_x")->GetText()), atoi(entity->FirstChildElement("target_y")->GetText()));
    }
    convoyList.emplace_back(convoy);
}

void MapLoader::addConvoyComponents(ConvoyObj* convoy, XMLElement* entity) {
    convoy->AddComponent(make_shared<renderComponent>(convoy, renderer, "draftArt/basicConvoy.png"));
    convoy->AddComponent(make_shared<buttonComponent>(convoy));
    convoy->AddComponent(make_shared<movementComponent>(convoy, 100));
    vector<int> max = { 100, 100, 100, 100, 100 };
    vector<int> count = { 50, 50, 50, 50, 50 };
    if (entity->FirstChildElement("Resources")) {
        XMLElement* resources = entity->FirstChildElement("Resources");
        count[PERSONNEL] = atoi(resources->FirstChildElement("Personnel")->GetText());
        count[AMMUNITION] = atoi(resources->FirstChildElement("Ammunition")->GetText());
        count[DOS] = atoi(resources->FirstChildElement("DoS")->GetText());
        count[FUEL] = atoi(resources->FirstChildElement("Fuel")->GetText());
        count[SCRAP] = atoi(resources->FirstChildElement("Scrap")->GetText());
    }
    convoy->AddComponent(make_shared<resourceComponent>(convoy, max, count, loadResourceTextures()));
	vector<int> transferRate = { 5,5,5,5,5 };
    convoy->AddComponent(make_shared<resourceTransferComponent>(convoy, renderer, 50, transferRate));
    convoy->AddComponent(make_shared<pathfindingComponent>(convoy, grid));
}

void MapLoader::loadZombie(XMLElement* entity)
{
    int x = atoi(entity->FirstChildElement("x")->GetText());
    int y = atoi(entity->FirstChildElement("y")->GetText());
    int id = atoi(entity->FirstChildElement("id")->GetText());
    int width = 1; // number of tiles
    int height = 1;
    ZombieObj* zombie = new ZombieObj(x, y, width, height, id);
    addZombieComponents(zombie, entity);
    if (entity->FirstChildElement("target_x")) {
        zombie->setTarget(atoi(entity->FirstChildElement("target_x")->GetText()), atoi(entity->FirstChildElement("target_y")->GetText()));
    }
    zombieList.emplace_back(zombie);
}

void MapLoader::addZombieComponents(ZombieObj* zombie, XMLElement* entity) {
    zombie->AddComponent(make_shared<renderComponent>(zombie, renderer, "draftArt/basicZombie.png"));
    zombie->AddComponent(make_shared<movementComponent>(zombie, 10));
    zombie->AddComponent(make_shared<pathfindingComponent>(zombie, grid));
	zombie->AddComponent(make_shared<nearestComponent>(zombie));
}


void MapLoader::loadDepot(XMLElement* entity)
{
    int x = atoi(entity->FirstChildElement("x")->GetText());
    int y = atoi(entity->FirstChildElement("y")->GetText());
    int width = 4; // number of tiles 
    int height = 4;
    depot = new DepotObj(x, y, width, height);
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
    depot->getComponent<resourceComponent>()->setResourceIncrease(PERSONNEL, 5);
    depot->getComponent<resourceComponent>()->setResourceIncrease(AMMUNITION, 5);
    depot->getComponent<resourceComponent>()->setResourceIncrease(DOS, 5);
    depot->getComponent<resourceComponent>()->setResourceIncrease(FUEL, 5);
    depot->getComponent<resourceComponent>()->setResourceIncrease(SCRAP, 5);
}

vector<SDL_Texture*> MapLoader::loadResourceTextures() {
    vector<SDL_Texture*> resourceTextures;
    vector<const char*> resourceFiles = {
        "draftArt/resources/Personnel.png",
        "draftArt/resources/Ammo.png",
        "draftArt/resources/DoS.png",
        "draftArt/resources/Fuel.png",
        "draftArt/resources/Scrap.png"
    };
    for (int i = 0; i < resourceFiles.size(); i++) {
        SDL_Surface* surface = IMG_Load(resourceFiles[i]);
        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_SetTextureScaleMode(texture, SDL_SCALEMODE_PIXELART);
        SDL_DestroySurface(surface); // Free the surface after creating the texture
        resourceTextures.push_back(texture);
    }
    return resourceTextures;
}

void MapLoader::renderTileMap(SDL_Renderer* renderer) {
    SDL_FRect destRect{-camera.dimen.x,-camera.dimen.y,tileWidth,tileHeight };
    for (auto& row : tilemap) {
        for (auto& column : row) {
            SDL_RenderTexture(renderer, tilemapTexture, &column, &destRect);
            //SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
			//SDL_RenderRect(renderer, &destRect);
            destRect.x += tileWidth;
        }
		destRect.x = -camera.dimen.x;
		destRect.y += tileHeight;
    }
}