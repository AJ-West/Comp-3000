#pragma once
#include <memory>
#include <SDL3/SDL.h>

#include "gameFiles/levelHandling/selectedHandler/handleSelected.h"

using namespace std;

class LevelManager;
class LevelUI;
class HumanObj;

class UnitSelected :public SelectedState {
public:
	UnitSelected(LevelManager* lManager, HumanObj* unit, HandleSelected* handleS, levelUI* lUI);
	~UnitSelected();

	virtual void handleInput(SDL_Event event);

	void leftClick();

	void rightClick();
private:
	HumanObj* selected;

	levelUI* UI;
};