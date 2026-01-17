#include "selectedHandler.h"

#include "gameFiles/UI/levelUI.h"
#include "gameFiles/levelHandling/levelManager.h"

SelectedHandler::SelectedHandler(vector<GameObject*> objs, DepotObj* dep, levelUI* lUI) : allObjects(objs), depot(dep), UI(lUI)
{
}

SelectedHandler::~SelectedHandler()
{
}

void SelectedHandler::checkHover(SDL_Event event) {
    for (auto &obj : allObjects) {
        if (obj) {
            obj->checkHover(event.motion.x, event.motion.y);
            if (obj->getHovering()) {
                hoveredObj = obj;
                return;
            }
        }
    }
    depot->checkHover(event.motion.x, event.motion.y);
    if (depot->getHovering()) {
        hoveredObj = depot;
        return;
    }
	hoveredObj = nullptr;
}

void SelectedHandler::deselectObject() {
    selectedObject->onClick();
    selectedObject = nullptr;
}

void SelectedHandler::checkClick() {
    bool selectedSomething = false;
    if (selectedObject && hoveredObj) { // if something is selected
        if (selectedObject->getHovering()) { // if deselecting current object
			deselectObject();
            selectedSomething = true;
        }
        else {
            hoveredObj->onClick();
            selectedObject->onClick();
            selectedObject = hoveredObj;
            selectedSomething = true;
        }
    }
    else if (hoveredObj) {
        hoveredObj->onClick();
        if (typeid(*hoveredObj).name() == typeid(DepotObj).name()) {
            UI->createNewUnitBox(depot);
        }
        selectedObject = hoveredObj;
        selectedSomething = true;
    }
    if (!selectedSomething && selectedObject) { // clicked away
        selectedObject->clickAway();
    }
}

void SelectedHandler::rightClick(LevelManager* manager) {
	if (selectedObject) {
        if (typeid(*selectedObject).name() == typeid(ConvoyObj).name()) {
			if (!hoveredObj || typeid(*hoveredObj).name() == typeid(ConvoyObj).name()) {
				deselectObject();
				return;
            }
            else {
                UI->createTransferBox(hoveredObj, selectedObject);
                manager->setPaused(true);
            }
        }
        else {
			deselectObject();
        }
	}
}