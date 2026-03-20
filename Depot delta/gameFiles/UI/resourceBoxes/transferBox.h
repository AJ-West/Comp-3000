#pragma once

#include "gameFiles/UI/resourceBoxes/resourceBox.h"
#include "gameFiles/components/resourceTransferComponent.h"
#include "arrowButton.h"

class textInput;
class LevelManager;

class transferBox: public ResourceBox {
public:
	transferBox(LevelManager* lManager, SDL_FRect rSize, GameObject* sUnit, GameObject* sConvoy);
	~transferBox();

	virtual void addTextInputs();

	void addUnitInfo(GameObject* unit, bool second);

	void addArrows();

	virtual void transferDirectionButtonClicked(UIElement* elem);

private:
	GameObject* unit;
	GameObject* convoy;

	vector<arrowButton*> arrows;

	vector<const char*> initTransferVAFiles{ "soundEffects/voice acting/unitsconvoys/initiateTransfer/hha.wav", "soundEffects/voice acting/unitsconvoys/initiateTransfer/ri.wav", "soundEffects/voice acting/unitsconvoys/initiateTransfer/sto.wav" };
};