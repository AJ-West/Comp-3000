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
    cell_width = atoi(layer->FirstChildElement("gridCellWidth")->GetText());
    cell_height = atoi(layer->FirstChildElement("gridCellHeight")->GetText());
    x_cells = atoi(layer->FirstChildElement("gridCellsX")->GetText());
    y_cells = atoi(layer->FirstChildElement("gridCellsY")->GetText());
    XMLElement* data = layer->FirstChildElement("data");
	for (int i = 0; i < y_cells; i++) {
		vector<SDL_FRect> row;
		for (int j = 0; j < x_cells; j++) {
            div_t loc = div(atoi(data->GetText()), 8); // gettile value as int then conver to location on tilemap spritesheet
			SDL_FRect cell;
			cell.x = loc.rem * cell_width;
			cell.y = loc.quot * cell_height;
			cell.w = cell_width;
			cell.h = cell_height;
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
			UnitObj* unit = new UnitObj(x, y, id);
			unitList.emplace_back(unit);
		}
		entity = entity->NextSiblingElement("entities");
	}
}

void MapLoader::renderTileMap(SDL_Renderer* renderer) {
    SDL_FRect destRect{-camera.x,-camera.y,cell_width,cell_height};
    for (auto& row : tilemap) {
        for (auto& column : row) {
            SDL_RenderTexture(renderer, tilemapTexture, &column, &destRect);
            destRect.x += cell_width;
        }
		destRect.x = -camera.x;
		destRect.y += cell_height;
    }
}

void MapLoader::saveFile(const char* filename, vector<UnitObj*> units)
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
            for (auto& unit : units) {
                if (unit->getID() == atoi(entity->FirstChildElement("id")->GetText())) {
					entity->FirstChildElement("x")->SetText(static_cast<int>(unit->getDimensions().x));
					entity->FirstChildElement("y")->SetText(static_cast<int>(unit->getDimensions().y));
					break;
                }
            }
			entity = entity->NextSiblingElement("entities");
        }
		doc.SaveFile(filename);
    }
    else{
        cerr << "Unable to save file! " << endl;
    }
}