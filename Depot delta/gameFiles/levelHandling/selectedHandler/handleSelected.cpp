#pragma once
#include "gameFiles/levelHandling/selectedHandler/handleSelected.h"

#include "gameFiles/UI/levelUI.h"
#include "gameFiles/levelHandling/levelManager.h"
#include "gameFiles/entities/humanObject.h"
#include "gameFiles/entities/Depot/depotObject.h"

enum ObjectType convert(const char* str) {
	cout << str << "t" << '\n';
	cout << typeid(HumanObj).name() << "t" << '\n';
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
		enum ObjectType type = convert(typeid(*(hovered.get())).name());
		switch (type)
		{
		case Human:
			// downcasting check to pass correct object
			if (shared_ptr<HumanObj> human = static_pointer_cast<HumanObj>(hovered)) {
				setState(make_shared<UnitSelected>(manager, human.get(), hovered, UI));
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
				enum ObjectType type = convert(typeid(*obj).name());
				switch (type)
				{
				case Human:
					// downcasting check to pass correct object
					if (HumanObj* human = dynamic_cast<HumanObj*>(obj)) {
						hovered = make_shared<HumanObj>(*human);
					}
					break;
				case Depot:
					// downcasting check to pass correct object
					if (DepotObj* depot = dynamic_cast<DepotObj*>(obj)) {
						hovered = make_shared<DepotObj>(*depot);
					}
					break;
				default:
					hovered.reset();
					break;
				}
				return;
			}
		}
	}
	hovered.reset();
}

UnitSelected::UnitSelected(LevelManager* lManager, HumanObj* unit, shared_ptr<GameObject> hover, levelUI* lUI) : SelectedState(lManager), selected(unit), hovered(hover), UI(lUI) {
	//duplicates pointer to avoid unit changing when hovered changes
	unit = new HumanObj(*unit);
	selected = unit;
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
	if (!hovered) { // move unit to click pos
		selected->clickAway();
	}
	else { // if clicking on another object
		selected->onClick();
		deselect();
	}
}

void UnitSelected::rightClick() {
	if (hovered) { // if right clicking on another unit initiate transfer
		UI->createTransferBox(hovered.get(), selected);
		manager->setPaused(true);
	}
	else {  // unselect unit
		selected->onClick();
		deselect();
	}
}
