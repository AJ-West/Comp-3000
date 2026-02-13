#pragma once
#include "gameFiles/levelHandling/selectedHandler/states/UnitSelected.h"
#include "gameFiles/levelHandling/selectedHandler/states/transferState.h"

#include "gameFiles/UI/levelUI.h"
#include "gameFiles/levelHandling/levelManager.h"
#include "gameFiles/entities/humanObject.h"
#include "gameFiles/entities/Depot/depotObject.h"

UnitSelected::UnitSelected(LevelManager* lManager, HumanObj* unit, HandleSelected* handler, levelUI* lUI) : SelectedState(lManager, handler), selected(unit), UI(lUI) {
	selected->onClick();
}
UnitSelected::~UnitSelected() {}

void UnitSelected::handleInput(SDL_Event event) {
	if (selected) {
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
	else {
		deselect();
	}
}

void UnitSelected::leftClick() {
	if (!handler->getHovered()) { // move unit to click pos
		if (!UI->checkClickInput()) {
			selected->clickAway();
		}		
	}
	else { // if clicking on another object
		selected->onClick();
		deselect();
	}
}

void UnitSelected::rightClick() {
	selected->onClick();
	GameObject* hovered = handler->getHovered();
	if (hovered) { // if right clicking on another unit initiate transfer
		if (hovered != selected) {
			if (selected->getComponent<resourceTransferComponent>()->checkDistance(selected->getDimensions(), handler->getHovered()->getDimensions())) {
				handler->setStateEnum(selectTransfer);
				handler->setOrigin(selected);
				deselect();
			}
			return;
		}
	}
	deselect();
}
