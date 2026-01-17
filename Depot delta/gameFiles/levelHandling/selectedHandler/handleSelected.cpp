#pragma once
#include "gameFiles/levelHandling/selectedHandler/handleSelected.h"

#include "gameFiles/UI/levelUI.h"
#include "gameFiles/levelHandling/levelManager.h"
#include "gameFiles/entities/humanObject.h"
#include "gameFiles/entities/Depot/depotObject.h"

enum ObjectType convert(const char* str) {
	if (strcmp(str, typeid(HumanObj).name()) == 0) return Human;
	if (strcmp(str, typeid(UnitObj).name()) == 0) return Human;
	if (strcmp(str, typeid(ConvoyObj).name()) == 0) return Human;
	if (strcmp(str, typeid(DepotObj).name()) == 0) return Depot;
};

void HandleSelected::handleInput(SDL_Event event, LevelManager* manager) {
	checkHover(event);
	if (currentState) {
		currentState->handleInput(event);
		if (currentState->endState()) { decideState(manager); } // sets to nullptr
	}
	else {
		if (hovered && event.button.button == SDL_BUTTON_LEFT) {
			decideState(manager);
		}
	}
}

void HandleSelected::decideState(LevelManager* manager) {
	if (hovered) {
		enum ObjectType type = convert(typeid(*hovered).name());
		switch (type)
		{
		case Human:
			// downcasting check to pass correct object
			if (HumanObj* human = dynamic_cast<HumanObj*>(hovered)) {
				setState(make_shared<UnitSelected>(manager, human, this, UI));
			}
			break;
		default:
			currentState.reset();
			break;
		}
	}
	else {
		currentState.reset();
	}	
}

void HandleSelected::checkHover(SDL_Event event) {
	for (auto& obj : allObjects) {
		if (obj) {
			obj->checkHover(event.motion.x, event.motion.y);
			if (obj->getHovering()) {
				hovered = obj;
				return;
			}
		}
	}
	hovered = nullptr;
}

UnitSelected::UnitSelected(LevelManager* lManager, HumanObj* unit, HandleSelected* handler, levelUI* lUI) : SelectedState(lManager, handler), selected(unit), UI(lUI) {
	selected->onClick();
}
UnitSelected::~UnitSelected() {}

void UnitSelected::handleInput(SDL_Event event) {
	if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
		if (event.button.button == SDL_BUTTON_LEFT) {
			leftClick();
		}
		else if (event.button.button == SDL_BUTTON_RIGHT) {
			rightClick();
		}
	}
}

void UnitSelected::leftClick() {
	if (!handler->getHovered()) { // move unit to click pos
		selected->clickAway();
	}
	else { // if clicking on another object
		selected->onClick();
		deselect();
	}
}

void UnitSelected::rightClick() {
	if (handler->getHovered()) { // if right clicking on another unit initiate transfer
		if (selected->getComponent<resourceTransferComponent>()->checkDistance(selected->getDimensions(), handler->getHovered()->getDimensions())) {
			UI->createTransferBox(handler->getHovered(), selected);
			manager->setPaused(true);
		}
	}
	else {  // unselect unit
		selected->onClick();
		deselect();
	}
}
