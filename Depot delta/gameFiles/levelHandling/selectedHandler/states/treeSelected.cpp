#pragma once
#include "gameFiles/levelHandling/selectedHandler/states/treeSelected.h"

#include "gameFiles/UI/levelUI.h"
#include "gameFiles/levelHandling/levelManager.h"

TreeSelected::TreeSelected(LevelManager* lManager, HandleSelected* handler, levelUI* lUI) : SelectedState(lManager, handler), UI(lUI) {
	UI->createTechTree();
	manager->setPaused(true);
}
TreeSelected::~TreeSelected() {}

void TreeSelected::handleInput(SDL_Event event) {
	if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
		if (event.button.button == SDL_BUTTON_LEFT) {
			leftClick();
		}
		else if (event.button.button == SDL_BUTTON_RIGHT) {
			rightClick();
		}
	}
}

void TreeSelected::leftClick() { // needs changing to handle input for tech tree
	UI->checkClickInput();
}

void TreeSelected::rightClick() {
	UI->deleteTree();
	deselect();
}
