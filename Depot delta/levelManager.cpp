#include "levelManager.h"


levelManager::levelManager(const char* filename)
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
    tilemap.resize(y_cells, vector<int>(x_cells, 0));
    int x_count = 0;
    int y_count = 0;
    data = layer->FirstChildElement("data");
    while (data) {
        tilemap[y_count][x_count] = atoi(data->GetText());
        x_count++;
        if (x_count == x_cells) {
            x_count == 0;
            y_count++;
        }
        data = data->NextSiblingElement("data");
    }
}

levelManager::~levelManager()
{
}