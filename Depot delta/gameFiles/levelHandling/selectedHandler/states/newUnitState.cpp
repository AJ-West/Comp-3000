#pragma once
#include "gameFiles/levelHandling/selectedHandler/states/newUnitState.h"

#include "gameFiles/UI/levelUI.h"
#include "gameFiles/levelHandling/levelManager.h"
#include "gameFiles/entities/Depot/depotObject.h"

NewUnitState::NewUnitState(LevelManager* lManager, GameObject* dep, HandleSelected* handler, levelUI* lUI) : SelectedState(lManager, handler), depot(dynamic_cast<DepotObj*>(dep)), UI(lUI) {
	UI->createNewUnitBox(depot, manager);
	depot->onClick();
	manager->setPaused(true);
}
NewUnitState::~NewUnitState() {}

void NewUnitState::handleInput(SDL_Event event) {
	if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
		if (event.button.button == SDL_BUTTON_LEFT) {
			leftClick();
		}
		else if (event.button.button == SDL_BUTTON_RIGHT) {
			rightClick();
		}
	}
	else if (event.type == SDL_EVENT_KEY_DOWN) {
		if (textInput) {
			UI->textInput(event.key.key);
			return;
		}
	}
}

void NewUnitState::leftClick() {
	if (UI->checkClickInput()) {
		UIElement* selectedElement = UI->getBox()->getSelectedElement();
		if (typeid(selectedElement).name() == typeid(textInput).name()) { textInput = true; }
		else { selectedElement->update(); }
		return;
	}
	if (!UI->getBox()) { // if box has been removed
		deselect();
	}
}

void NewUnitState::rightClick() {
	UI->deleteBox();
	depot->onClick();
	deselect();
}
