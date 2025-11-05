#include "levelManager.h"


LevelManager::LevelManager(const char* filename, SDL_Renderer* renderer)
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
    cout << layer->FirstChildElement("name")->GetText();
    cout << layer->FirstChildElement("offsetX")->GetText();
    cout << layer->FirstChildElement("offsetY")->GetText();
    cout << layer->FirstChildElement("gridCellWidth")->GetText();
    cout << layer->FirstChildElement("gridCellHeight")->GetText();
    cout << layer->FirstChildElement("gridCellsX")->GetText();
    cout << layer->FirstChildElement("gridCellsY")->GetText();
    cout << layer->FirstChildElement("tileset")->GetText();
    XMLElement* data = layer->FirstChildElement("data");

    width = atoi(root->FirstChildElement("width")->GetText());
    height = atoi(root->FirstChildElement("height")->GetText());
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
    }
	loadTileMap();

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

LevelManager::~LevelManager()
{
}

void LevelManager::loadTileMap()
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

void LevelManager::renderTileMap(SDL_Renderer* renderer) {
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