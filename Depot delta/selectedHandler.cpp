#include "selectedHandler.h"

SelectedHandler::SelectedHandler(vector<GameObject*> objs) : allObjects(objs)
{
}

SelectedHandler::~SelectedHandler()
{
}

void SelectedHandler::checkHover(SDL_Event event) {
    for (auto& obj : allObjects) {
        obj->checkHover(event.motion.x, event.motion.y);
		if (obj->getHovering()) {
            hoveredObj = obj;
            return;
        }
    }
	hoveredObj = nullptr;
}

void SelectedHandler::checkClick() {
    bool selectedSomething = false;
    if (selectedObject && hoveredObj) { // if something is selected
        if (selectedObject->getHovering()) { // if deselecting current object
            selectedObject->onClick();
            selectedObject = nullptr;
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
        selectedObject = hoveredObj;
        selectedSomething = true;
    }
    if (!selectedSomething && selectedObject) { // clicked away
        selectedObject->clickAway();
    }
}