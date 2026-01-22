#pragma once
#include <memory>
#include <SDL3/SDL.h>

#include "gameFiles/levelHandling/selectedHandler/handleSelected.h"

using namespace std;

class LevelManager;
class levelUI;
class HandleSelected;
class TechTree;

class TreeSelected :public SelectedState {
public:
	TreeSelected(LevelManager* lManager, HandleSelected* handleS, levelUI* lUI);
	~TreeSelected();

	virtual void handleInput(SDL_Event event);

	void leftClick();

	void rightClick();
private:
	TechTree* tree;
	levelUI* UI;
};