#pragma once
#include <memory>
#include <SDL3/SDL.h>

#include "gameFiles/useThroughout/variables.h"

using namespace std;

class UnitSelected;
class LevelManager;
class LevelUI;
class HumanObj;
class DepotObj;
class GameObject; 
class levelUI;
class HandleSelected;

class SelectedState {
public:
	SelectedState(LevelManager* lManager, HandleSelected* handleS) : manager(lManager), handler(handleS){}
	~SelectedState(){}

	virtual void handleInput(SDL_Event event) = 0;

	void deselect() { destroy = true; }
	bool endState() { return destroy; }
protected:
	bool destroy = false;
	LevelManager* manager;
	HandleSelected* handler;
};

class HandleSelected {
public:
	HandleSelected(vector<GameObject*> objects, levelUI* lUI) : allObjects(objects), UI(lUI) {}
	~HandleSelected() {}
	
	void setState(shared_ptr<SelectedState> state) { currentState = state; }

	void handleInput(SDL_Event event, LevelManager* manager);

	void decideState(LevelManager* manager);

	void checkHover(SDL_Event event);

	GameObject* getHovered() { return hovered; }

private:
	shared_ptr<SelectedState> currentState;

	GameObject* hovered = nullptr;

	vector<GameObject*> allObjects;
	levelUI* UI;
};

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