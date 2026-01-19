#pragma once

#include "gameFiles/UI/resourceBoxes/resourceBox.h"
#include "gameFiles/components/resourceTransferComponent.h"

class textInput;
class LevelManager;

class transferBox: public ResourceBox {
public:
	transferBox(LevelManager* lManager, SDL_FRect rSize, GameObject* sUnit, GameObject* sConvoy);
	~transferBox();

	void addUnitInfo(GameObject* unit, bool second);

	virtual void transferDirectionButtonClicked(UIElement* elem);

private:
	GameObject* unit;
	GameObject* convoy;
};