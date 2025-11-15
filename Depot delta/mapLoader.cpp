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
			int x = atoi(entity->FirstChildElement("x")->GetText());
			int y = atoi(entity->FirstChildElement("y")->GetText());
			int id = atoi(entity->FirstChildElement("id")->GetText());
            int width = atoi(entity->FirstChildElement("width")->GetText())/2; // div 2 as ogmo editor does not allow value of 1
            int height = atoi(entity->FirstChildElement("height")->GetText())/2;
			UnitObj* unit = new UnitObj(x, y, width, height, id);
			unitList.emplace_back(unit);
            if (entity->FirstChildElement("target_x")) {
                unit->setTarget(atoi(entity->FirstChildElement("target_x")->GetText()), atoi(entity->FirstChildElement("target_y")->GetText()));
            }
		}
        if (name == "depot") {
            int x = atoi(entity->FirstChildElement("x")->GetText());
            int y = atoi(entity->FirstChildElement("y")->GetText());
            int width = atoi(entity->FirstChildElement("width")->GetText()) / 2; // div 2 as ogmo editor does not allow value of 1
            int height = atoi(entity->FirstChildElement("height")->GetText()) / 2;
            depot = new DepotObj(x, y, width, height);
        }
		entity = entity->NextSiblingElement("entities");
	}
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