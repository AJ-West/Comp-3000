#pragma once
#include "gameFiles/levelHandling/selectedHandler/states/transferState.h"

#include "gameFiles/UI/levelUI.h"
#include "gameFiles/levelHandling/levelManager.h"
#include "gameFiles/entities/humanObject.h"
#include "gameFiles/entities/Depot/depotObject.h"

TransferState::TransferState(LevelManager* lManager, GameObject* unit, GameObject* unit2, HandleSelected* handler, levelUI* lUI) : SelectedState(lManager, handler), obj1(unit), obj2(unit2), UI(lUI) {
	UI->createTransferBox(obj1, obj2);
	manager->setPaused(true);
}
TransferState::~TransferState() {}

void TransferState::handleInput(SDL_Event event) {
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

void TransferState::leftClick() {
	if (UI->checkClickInput()) {
		UIElement* selectedElement = UI->getBox()->getSelectedElement();
		if (typeid(selectedElement).name() == typeid(textInput).name()) { textInput = true; }
		else if (typeid(*selectedElement).name() == typeid(transferDirectionButton).name()) { // if new unit button
			UI->deleteBox();
			deselect();
		}
		else { selectedElement->update(); }
		return;
	}
	if (!UI->getBox()) { // if box has been removed
		deselect();
	}
}

void TransferState::rightClick() {
	textInput = false;
	UI->deleteBox();
	deselect();
}
