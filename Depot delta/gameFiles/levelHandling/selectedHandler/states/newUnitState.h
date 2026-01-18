#pragma once
#include <memory>
#include <SDL3/SDL.h>

#include "gameFiles/levelHandling/selectedHandler/handleSelected.h"

using namespace std;

class LevelManager;
class GameObject; 
class levelUI;
class HandleSelected;
class DepotObj;

class NewUnitState :public SelectedState {
public:
	NewUnitState(LevelManager* lManager, GameObject* depot, HandleSelected* handleS, levelUI* lUI);
	~NewUnitState();

	virtual void handleInput(SDL_Event event);

	void leftClick();

	void rightClick();
private:
	DepotObj* depot;

	levelUI* UI;

	bool textInput;
};