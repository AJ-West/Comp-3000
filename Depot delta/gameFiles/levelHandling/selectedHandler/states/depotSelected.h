#pragma once
#include <memory>
#include <SDL3/SDL.h>

#include "gameFiles/levelHandling/selectedHandler/handleSelected.h"

using namespace std;

class UnitSelected;
class LevelManager;
class LevelUI;
class HumanObj;
class DepotObj;
class GameObject; 
class levelUI;
class HandleSelected;

class DepotSelected :public SelectedState {
public:
	DepotSelected(LevelManager* lManager, DepotObj* dep, HandleSelected* handleS, levelUI* lUI);
	~DepotSelected();

	virtual void handleInput(SDL_Event event);

	void leftClick();

	void rightClick();
private:
	DepotObj* depot;

	levelUI* UI;
};