#pragma once
#include "gameFiles/levelHandling/selectedHandler/states/depotSelected.h"

#include "gameFiles/UI/levelUI.h"
#include "gameFiles/levelHandling/levelManager.h"
#include "gameFiles/entities/humanObject.h"
#include "gameFiles/entities/Depot/depotObject.h"


DepotSelected::DepotSelected(LevelManager* lManager, DepotObj* dep, HandleSelected* handler, levelUI* lUI) : SelectedState(lManager, handler), depot(dep), UI(lUI) {
	depot->onClick();
	UI->createNewUnitBox(depot);
	manager->setPaused(true);
}
DepotSelected::~DepotSelected() {}

void DepotSelected::handleInput(SDL_Event event) {
	if (event.type == SDL_EVENT_MOUSE_WHEEL) {
		manager->zoomChange(event);
	}
	else if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
		if (event.button.button == SDL_BUTTON_LEFT) {
			leftClick();
		}
		else if (event.button.button == SDL_BUTTON_RIGHT) {
			rightClick();
		}
	}
	else if (event.type == SDL_EVENT_KEY_DOWN) {
		camera.keyDown(event.key.key);
	}
	else if (event.type == SDL_EVENT_KEY_UP) {
		camera.keyUp(event.key.key);
	}
}

void DepotSelected::leftClick() { // needs changing for input on new unit box
	depot->onClick();
	deselect();
}

void DepotSelected::rightClick() {
	depot->onClick();
	deselect();
}
