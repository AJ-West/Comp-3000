#include "transferBox.h"
#include "UnitStats.h"

transferBox::transferBox(LevelManager* lManager, SDL_FRect rSize, GameObject* sUnit, GameObject* sConvoy) : ResourceBox(lManager, rSize), unit(sUnit), convoy(sConvoy) {
	//add convoy->unit button
	elements.push_back(new transferDirectionButton({ size.x + 415.0f * camera.xScale, size.y + 788.0f * camera.yScale, 170.0f * camera.xScale , 65.0f * camera.yScale }, true));
	//add unit->convoy button
	//elements.push_back(new transferDirectionButton({ size.x + 394.0f * camera.xScale, size.y + 140.0f * camera.yScale, 192.0f * camera.xScale , 48.0f * camera.yScale }, false));
	addUnitInfo(unit, false);
	addUnitInfo(convoy, true);
}
transferBox::~transferBox() {}

void transferBox::addUnitInfo(GameObject* unit, bool second) {
	if (second) {
		SDL_FRect pos{ size.x + 539 * camera.xScale, size.y + 222 * camera.yScale, 303 * camera.xScale, 562 * camera.yScale };
		elements.push_back(new UnitStats(unit, pos));
		return;
	}
	SDL_FRect pos{ size.x + 157 * camera.xScale, size.y + 222 * camera.yScale, 318 * camera.xScale, 562 * camera.yScale };
	elements.push_back(new UnitStats(unit, pos));
}

void transferBox::transferDirectionButtonClicked(UIElement* elem) {
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
