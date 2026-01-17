#pragma once
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <SDL3_image/SDL_image.h>

#include "dependencies/tinyxml2.h"
#include "gameFiles/fileHandling/mapLoader.h"
#include "gameFiles/fileHandling/mapSaver.h"

#include "gameFiles/entities/gameObject.h"
#include "gameFiles/entities/Convoys/convoyObject.h"

#include "gameFiles/components/resourceTransferComponent.h"

#include "gameFiles/useThroughout/variables.h"

using namespace std;
using namespace tinyxml2;

class levelUI;
class LevelManager;

class SelectedHandler {
public:
    SelectedHandler(vector<GameObject*> objs, DepotObj* dep, levelUI* lUI);
	~SelectedHandler();

	void checkHover(SDL_Event event);

	void checkClick();
	void rightClick(LevelManager* manager);

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
