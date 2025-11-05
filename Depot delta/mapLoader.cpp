#include "mapLoader.h"


MapLoader::MapLoader(const char* filename, SDL_Renderer* renderer)
{
    XMLDocument doc;
    doc.LoadFile(filename);
    XMLElement* root = doc.RootElement();
    cout << root->FirstChildElement("ogmoVersion")->GetText();
    cout << root->FirstChildElement("width")->GetText();
    cout << root->FirstChildElement("height")->GetText();
    cout << root->FirstChildElement("offsetX")->GetText();
    cout << root->FirstChildElement("offsetY")->GetText();
    XMLElement* layer = root->FirstChildElement("layers");
    while (layer) {
        string layerName = string(layer->FirstChildElement("name")->GetText());
        if (layerName == "Tilemap") {
            loadTilemap(layer);
        }
		layer = layer->NextSiblingElement("layer");
       // else if (layerName == "Entities") {
         //   loadEntities(layer);
        //}

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
    /*XMLElement* data = layer->FirstChildElement("data");
    cell_width = atoi(layer->FirstChildElement("gridCellWidth")->GetText());
    cell_height = atoi(layer->FirstChildElement("gridCellHeight")->GetText());
    x_cells = atoi(layer->FirstChildElement("gridCellsX")->GetText());
    y_cells = atoi(layer->FirstChildElement("gridCellsY")->GetText());
    cout << y_cells;
    cout << x_cells;
    tilelocs.resize(y_cells, vector<int>(x_cells, 0));
    int x_count = 0;
    int y_count = 0;
    data = layer->FirstChildElement("data");
    while (data) {
        tilelocs[y_count][x_count] = atoi(data->GetText());
        x_count++;
        if (x_count == x_cells) {
            x_count = 0;
            y_count++;
        }
        data = data->NextSiblingElement("data");
    }*/
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
    for (int i = 0; i < y_cells; i++) {
        vector<SDL_FRect> row;
        for (int j = 0; j < x_cells; j++) {
            div_t loc = div(tilelocs[i][j], 8); // Use tilelocs as needed
            SDL_FRect cell;
            cell.x = loc.rem * cell_width;
            cell.y = loc.quot * cell_height;
            cell.w = cell_width;
            cell.h = cell_height;
            row.push_back(cell);
        }
        tilemap.push_back(row);
    }
}

void MapLoader::renderTileMap(SDL_Renderer* renderer) {
    SDL_FRect destRect{0,0,cell_width,cell_height};
    for (auto& row : tilemap) {
        for (auto& column : row) {
            SDL_RenderTexture(renderer, tilemapTexture, &column, &destRect);
            destRect.x += cell_width;
        }
		destRect.x = 0;
		destRect.y += cell_height;
    }
}