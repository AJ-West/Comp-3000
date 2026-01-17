#pragma once

#include "gameFiles/UI/resourceBoxes/resourceBox.h"
#include "gameFiles/components/resourceTransferComponent.h"

class textInput;
class LevelManager;

class transferBox: public ResourceBox {
public:
	transferBox(LevelManager* lManager, SDL_FRect rSize, GameObject* sUnit, GameObject* sConvoy) : ResourceBox(lManager, rSize), unit(sUnit), convoy(sConvoy) {
		//add convoy->unit button
		elements.push_back(new transferDirectionButton({ size.x + 74.0f * camera.xScale, size.y + 140.0f * camera.yScale, 192.0f * camera.xScale , 48.0f * camera.yScale }, true));
		//add unit->convoy button
		elements.push_back(new transferDirectionButton({ size.x + 394.0f * camera.xScale, size.y + 140.0f * camera.yScale, 192.0f * camera.xScale , 48.0f * camera.yScale }, false));
	}
	~transferBox(){}

	virtual void transferDirectionButtonClicked(UIElement* elem) { 
		vector<int> amounts;
		for (auto elem : elements) { // get value of each resource input
			if (typeid(*elem).name() == typeid(textInput).name()) {
				if (elem->getText() != "") {
					amounts.push_back(stoi(elem->getText()));
				}
				else {
					amounts.push_back(0);
				}
			}
		}
		if (elem->update(NULL)) {
			convoy->getComponent<resourceTransferComponent>()->initiateTransfer(unit, amounts);
		}
		else {
			unit->getComponent<resourceTransferComponent>()->initiateTransfer(convoy, amounts);
		}
		unPause();
		//should remove transfer box on beginning of transfer
		toDelete = true;
	}

private:
	GameObject* unit;
	GameObject* convoy;
};