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
    x_cells = atoi(layer->FirstChildElement("gridCellsX")->GetText());
    y_cells = atoi(layer->FirstChildElement("gridCellsY")->GetText());
    XMLElement* data = layer->FirstChildElement("data");
	for (int i = 0; i < y_cells; i++) {
		vector<SDL_FRect> row;
		for (int j = 0; j < x_cells; j++) {
            div_t loc = div(atoi(data->GetText()), 8); // gettile value as int then conver to location on tilemap spritesheet
			SDL_FRect cell;
			cell.x = loc.rem * tileWidth;
			cell.y = loc.quot * tileHeight;
			cell.w = tileWidth;
			cell.h = tileHeight;
			row.push_back(cell);
            data = data->NextSiblingElement("data");
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
		entity = entity->NextSiblingElement("entities");
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
    if (entity->FirstChildElement("target_x")) {
        unit->setTarget(atoi(entity->FirstChildElement("target_x")->GetText()), atoi(entity->FirstChildElement("target_y")->GetText()));
    }
    addUnitComponents(unit, entity);
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
}

void MapLoader::loadConvoy(XMLElement* entity)
{
    int x = atoi(entity->FirstChildElement("x")->GetText());
    int y = atoi(entity->FirstChildElement("y")->GetText());
    int id = atoi(entity->FirstChildElement("id")->GetText());
    int width = 1; // number of tiles
    int height = 1;
    ConvoyObj* convoy = new ConvoyObj(x, y, width, height, id);
    if (entity->FirstChildElement("target_x")) {
        convoy->setTarget(atoi(entity->FirstChildElement("target_x")->GetText()), atoi(entity->FirstChildElement("target_y")->GetText()));
    }
    addConvoyComponents(convoy, entity);
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
    SDL_FRect destRect{-camera.x,-camera.y,tileWidth,tileHeight };
    for (auto& row : tilemap) {
        for (auto& column : row) {
            SDL_RenderTexture(renderer, tilemapTexture, &column, &destRect);
            destRect.x += tileWidth;
        }
		destRect.x = -camera.x;
		destRect.y += tileHeight;
    }
}

void MapLoader::saveFile(const char* filename, vector<UnitObj*> units, DepotObj* depot)
{
    XMLDocument doc;
    doc.LoadFile(filename);
    XMLElement* root = doc.RootElement();
    XMLElement* layer = root->FirstChildElement("layers");
    bool found = false;
    while (!found && layer) {
		if (string(layer->FirstChildElement("name")->GetText()) == "Entities") {
			found = true;
			break;
		}
		layer = layer->NextSiblingElement("layers");
    }
    if (found) {
        XMLElement* entity = layer->FirstChildElement("entities");
        while (entity) {
            string name = string(entity->FirstChildElement("name")->GetText());
            if (name == "Basic unit") {
                save_unit(entity, doc, units);
            }
            else if (name == "depot") {
                save_depot(entity, doc, depot);
            }
			entity = entity->NextSiblingElement("entities");
        }
		doc.SaveFile(filename);
    }
    else{
        cerr << "Unable to save file! " << endl;
    }
}

void MapLoader::save_unit(XMLElement* entity, XMLDocument& doc, vector<UnitObj*> units)
{
    for (auto& unit : units) {
        if (unit->getID() == atoi(entity->FirstChildElement("id")->GetText())) {
            entity->FirstChildElement("x")->SetText(static_cast<int>(unit->getDimensions().x));
            entity->FirstChildElement("y")->SetText(static_cast<int>(unit->getDimensions().y));
            if (entity->FirstChildElement("target_x")) {
                entity->FirstChildElement("target_x")->SetText(unit->getTargetPos().first);
                entity->FirstChildElement("target_y")->SetText(unit->getTargetPos().second);
            }
            else {
                XMLElement* target = doc.NewElement("target_x");
                target->SetText(unit->getTargetPos().first);
                entity->InsertAfterChild(entity->FirstChildElement("originY"), target);
                target = doc.NewElement("target_y");
                target->SetText(unit->getTargetPos().second);
                entity->InsertAfterChild(entity->FirstChildElement("target_x"), target);
            }
            save_resources(entity, doc, unit);
            break;
        }
    }
}

void MapLoader::save_convoy(XMLElement* entity, XMLDocument& doc, vector<ConvoyObj*> convoys)
{
    for (auto& convoy : convoys) {
        if (convoy->getID() == atoi(entity->FirstChildElement("id")->GetText())) {
            entity->FirstChildElement("x")->SetText(static_cast<int>(convoy->getDimensions().x));
            entity->FirstChildElement("y")->SetText(static_cast<int>(convoy->getDimensions().y));
            if (entity->FirstChildElement("target_x")) {
                entity->FirstChildElement("target_x")->SetText(convoy->getTargetPos().first);
                entity->FirstChildElement("target_y")->SetText(convoy->getTargetPos().second);
            }
            else {
                XMLElement* target = doc.NewElement("target_x");
                target->SetText(convoy->getTargetPos().first);
                entity->InsertAfterChild(entity->FirstChildElement("originY"), target);
                target = doc.NewElement("target_y");
                target->SetText(convoy->getTargetPos().second);
                entity->InsertAfterChild(entity->FirstChildElement("target_x"), target);
            }
            save_resources(entity, doc, convoy);
            break;
        }
    }
}


void MapLoader::save_depot(XMLElement* entity, XMLDocument& doc, DepotObj* depot)
{
	save_resources(entity, doc, depot);
}

void MapLoader::save_resources(XMLElement* entity, XMLDocument& doc, GameObject* obj) {
	shared_ptr<resourceComponent> rC = obj->getComponent<resourceComponent>();
    if (entity->FirstChildElement("Resources")) {
		XMLElement* resources = entity->FirstChildElement("Resources");
        resources->FirstChildElement("Personnel")->SetText(rC->getResourcesCount(PERSONNEL));
        resources->FirstChildElement("Ammunition")->SetText(rC->getResourcesCount(AMMUNITION));
        resources->FirstChildElement("DoS")->SetText(rC->getResourcesCount(DOS));
        resources->FirstChildElement("Fuel")->SetText(rC->getResourcesCount(FUEL));
        resources->FirstChildElement("Scrap")->SetText(rC->getResourcesCount(SCRAP));
    }
    else {
        XMLElement* resources = doc.NewElement("Resources");
        XMLElement* personnel = doc.NewElement("Personnel");
        XMLElement* ammunition = doc.NewElement("Ammunition");
        XMLElement* dos = doc.NewElement("DoS");
        XMLElement* fuel = doc.NewElement("Fuel");
        XMLElement* scrap = doc.NewElement("Scrap");
		personnel->SetText(rC->getResourcesCount(PERSONNEL));
		ammunition->SetText(rC->getResourcesCount(AMMUNITION));
		dos->SetText(rC->getResourcesCount(DOS));
		fuel->SetText(rC->getResourcesCount(FUEL));
		scrap->SetText(rC->getResourcesCount(SCRAP));
		resources->InsertEndChild(personnel);
		resources->InsertEndChild(ammunition);
		resources->InsertEndChild(dos);
		resources->InsertEndChild(fuel);
		resources->InsertEndChild(scrap);
		entity->InsertEndChild(resources);
    }
}