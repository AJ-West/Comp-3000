#pragma once
#include <memory>
#include <SDL3/SDL.h>

#include "gameFiles/levelHandling/selectedHandler/handleSelected.h"

using namespace std;

class LevelManager;
class LevelUI;
class GameObject; 
class levelUI;
class HandleSelected;

class TransferState :public SelectedState {
public:
	TransferState(LevelManager* lManager, GameObject* unit, GameObject* unit2, HandleSelected* handleS, levelUI* lUI);
	~TransferState();

	virtual void handleInput(SDL_Event event);

	void leftClick();

	void rightClick();
private:
	GameObject* obj1;
	GameObject* obj2;

	levelUI* UI;

	bool textInput;
};