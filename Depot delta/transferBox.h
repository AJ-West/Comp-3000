#pragma once

#include "resourceBox.h"
#include "resourceTransferComponent.h"

class textInput;

class transferBox: public ResourceBox {
public:
	transferBox(SDL_FRect rSize, GameObject* sUnit, GameObject* sConvoy) : ResourceBox(rSize), unit(sUnit), convoy(sConvoy) {
		//add convoy->unit button
		elements.push_back(new transferDirectionButton({ size.x + 74.0f / 1440.0f * camera.dimen.w, size.y + 140.0f / 960.0f * camera.dimen.h, 192.0f / 1440.0f * camera.dimen.w , 48.0f / 960.0f * camera.dimen.h }, true));
		//add unit->convoy button
		elements.push_back(new transferDirectionButton({ size.x + 394.0f / 1440.0f * camera.dimen.w, size.y + 140.0f / 960.0f * camera.dimen.h, 192.0f / 1440.0f * camera.dimen.w , 48.0f / 960.0f * camera.dimen.h }, false));
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
		//should remove transfer box on beginning of transfer
		toDelete = true;
	}

private:
	GameObject* unit;
	GameObject* convoy;
};