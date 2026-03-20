#pragma once
#include <iostream>
#include <string>

#include "UIHandler.h"
#include "gameFiles/UI/resourceBoxes/resourceBox.h"

#include "gameFiles/UI/resourceBoxes/unitMaker.h"
#include "gameFiles/UI/resourceBoxes/transferBox.h"
#include "gameFiles/misc/dayCycle.h"
#include "gameFiles/levelHandling/levelManager.h"
#include "gameFiles/UI/Minimap.h"

using namespace std;

class GameObject;
class TechTree;

class levelUI : public UIHandler {
public:
	levelUI(SDL_Renderer* renderer, const char* filePath, LevelManager* lManager, dayCycle* gameTime);
	~levelUI();

	void loadResourceHoverTexture();

	void renderResourceHover();

	void renderTime(bool swarm);

	void render();

	void textInput(SDL_Keycode key);

	void createTransferBox(GameObject* sUnit, GameObject* sConvoy);
	void createNewUnitBox(DepotObj* gameDepot, LevelManager* lManager);

	void createTechTree();

	bool checkClickInput();

	void deleteBox();
	void deleteTree();

	ResourceBox* getBox() { return box; }
	TechTree* getTree() { return tree; }

private:
	SDL_Texture* resourceHoverTexture;

	Minimap* minimap;

	LevelManager* manager;

	dayCycle* time;	

	ResourceBox* box = nullptr;
	TechTree* tree = nullptr;
};