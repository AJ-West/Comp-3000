#pragma once
#include "gameFiles/levelHandling/selectedHandler/handleSelected.h"

#include "gameFiles/UI/levelUI.h"
#include "gameFiles/levelHandling/levelManager.h"
#include "gameFiles/entities/humanObject.h"
#include "gameFiles/entities/Depot/depotObject.h"

#include "gameFiles/levelHandling/selectedHandler/states/UnitSelected.h"
#include "gameFiles/levelHandling/selectedHandler/states/DepotSelected.h"
#include "gameFiles/levelHandling/selectedHandler/states/transferState.h"
#include "gameFiles/levelHandling/selectedHandler/states/newUnitState.h"
#include "gameFiles/levelHandling/selectedHandler/states/treeSelected.h"

enum ObjectType convert(const char* str) {
	if (strcmp(str, typeid(HumanObj).name()) == 0) return Human;
	if (strcmp(str, typeid(UnitObj).name()) == 0) return Human;
	if (strcmp(str, typeid(ConvoyObj).name()) == 0) return Human;
	if (strcmp(str, typeid(DepotObj).name()) == 0) return Depot;
};

void HandleSelected::handleInput(SDL_Event event, LevelManager* manager) {
	checkHover(event, manager);
	if (currentState) {
		//if (stateEnum == selectHuman) {

		//}
		currentState->handleInput(event);
		if (currentState->endState()) { decideState(manager); } // sets to nullptr
	}
	else {
		if (hovered && event.button.button == SDL_BUTTON_LEFT && event.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
			stateEnum = selectHuman;
			decideState(manager);
		}
		else if (event.type == SDL_EVENT_MOUSE_WHEEL) {	
			manager->zoomChange(event);
		}
		else if (event.type == SDL_EVENT_KEY_DOWN && event.key.key == SDLK_TAB) {
			stateEnum = selectTree;
			decideState(manager);
		}
		else if (event.type == SDL_EVENT_KEY_DOWN) {
			camera.keyDown(event.key.key);
		}
		else if (event.type == SDL_EVENT_KEY_UP) {
			camera.keyUp(event.key.key);
		}
	}
}

void HandleSelected::decideState(LevelManager* manager) {
	switch (stateEnum) {
	case selectHuman:
	case selectDepot:
		if (hovered) { // if clicked on different unit
			enum ObjectType type = convert(typeid(*hovered).name());
			switch (type)
			{
			case Human:
				// downcasting check to pass correct object
				if (HumanObj* human = dynamic_cast<HumanObj*>(hovered)) {
					setState(make_shared<UnitSelected>(manager, human, this, UI));
				}
				break;
			case Depot:
				// downcasting check to pass correct object
				if (DepotObj* depot = dynamic_cast<DepotObj*>(hovered)) {
					setState(make_shared<NewUnitState>(manager, depot, this, UI));
				}
				break;
			default:
				currentState = nullptr;
				break;
			}
		}
		else {
			currentState = nullptr;
		}
		break;
	case selectTransfer:
		setState(make_shared<TransferState>(manager, origin, hovered, this, UI));
		origin = nullptr;
		break;
	case selectNewUnit:
		setState(make_shared<NewUnitState>(manager, hovered, this, UI));
		break;
	case selectTree:
		setState(make_shared<TreeSelected>(manager, this, UI));
		break;
	default:
		currentState = nullptr;
		break;
	}
	stateEnum = INT8_MAX;	
}

void HandleSelected::checkHover(SDL_Event event, LevelManager* manager) {
	for (auto& obj : *manager->getUnitConvoys()) {
		if (obj) {
			//if(obj->checkHover(event.motion.x, event.motion.y)){
			if (obj->getHovering()) {
				hovered = obj.get();
				return;
			}
		}
	}
	if (auto obj = manager->getDepot()) {
		if (obj->getHovering()) {
			hovered = obj.get();
			return;
		}
	}
	hovered = nullptr;
}