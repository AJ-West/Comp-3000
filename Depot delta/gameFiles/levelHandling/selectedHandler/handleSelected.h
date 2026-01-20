#pragma once
#include <memory>
#include <SDL3/SDL.h>

#include "gameFiles/useThroughout/variables.h"

using namespace std;

class UnitSelected;
class LevelManager;
class levelUI;
class HumanObj;
class DepotObj;
class GameObject; 
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

	void setStateEnum(int state) { stateEnum = state; }
	void setOrigin(GameObject* org) { origin = org; }
	void setAllObjects(vector<GameObject*> objects) { allObjects = objects; }

private:
	shared_ptr<SelectedState> currentState;

	GameObject* hovered = nullptr;
	GameObject* origin = nullptr;

	vector<GameObject*> allObjects;
	levelUI* UI;

	int stateEnum = INT8_MAX;
};