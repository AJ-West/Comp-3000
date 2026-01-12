#pragma once
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <SDL3_image/SDL_image.h>

#include "tinyxml2.h"
#include "mapLoader.h"
#include "mapSaver.h"

#include "gameObject.h"
#include "convoyObject.h"

#include "resourceTransferComponent.h"

#include "variables.h"

using namespace std;
using namespace tinyxml2;

class levelUI;

class SelectedHandler {
public:
    SelectedHandler(vector<GameObject*> objs, DepotObj* dep, levelUI* lUI);
	~SelectedHandler();

	void checkHover(SDL_Event event);

	void checkClick();
	void rightClick();

	void deselectObject();

	//setters
	void setSelected(GameObject* selected) { selectedObject = selected; }
	void setAllObjects(vector<GameObject*> objects) { allObjects = objects; }

private:
	GameObject* selectedObject = nullptr;
    GameObject* hoveredObj = nullptr;

	DepotObj* depot;

	vector<GameObject*> allObjects;

	levelUI* UI;
};
