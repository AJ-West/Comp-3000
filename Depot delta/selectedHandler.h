#pragma once
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <SDL3_image/SDL_image.h>

#include "tinyxml2.h"
#include "mapLoader.h"
#include "mapSaver.h"

#include "gameObject.h"

#include "variables.h"

using namespace std;
using namespace tinyxml2;

class SelectedHandler {
public:
    SelectedHandler(vector<GameObject*> objs);
	~SelectedHandler();

	void checkHover(SDL_Event event);
	void checkClick();

private:
	SDL_Renderer* renderer;

	GameObject* selectedObject = nullptr;
    GameObject* hoveredObj = nullptr;

	vector<GameObject*> allObjects;
};
