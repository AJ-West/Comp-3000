#pragma once
#include <iostream>
#include <string>

#include "UIHandler.h"
#include "gameFiles/UI/resourceBoxes/resourceBox.h"

#include "gameFiles/UI/resourceBoxes/unitMaker.h"
#include "gameFiles/UI/resourceBoxes/transferBox.h"
#include "gameFiles/misc/dayCycle.h"
#include "gameFiles/levelHandling/levelManager.h"

using namespace std;

class GameObject;

class levelUI : public UIHandler {
public:
	levelUI(SDL_Renderer* renderer, const char* filePath, LevelManager* lManager, dayCycle* gameTime);
	~levelUI();

	void loadResourceHoverTexture();

	void renderResourceHover();

	void renderTime();

	void render();

	void textInput(SDL_Keycode key);

	void createTransferBox(GameObject* sUnit, GameObject* sConvoy);
	void createNewUnitBox(DepotObj* gameDepot);

	bool checkClickInput();

private:
	SDL_Texture* resourceHoverTexture;

	LevelManager* manager;

	dayCycle* time;	

	ResourceBox* box = nullptr;
};