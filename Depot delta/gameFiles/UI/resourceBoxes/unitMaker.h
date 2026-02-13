#pragma once

#include "gameFiles/levelHandling/levelManager.h"

#include "gameFiles/entities/Depot/depotObject.h"
#include "gameFiles/entities/Units/unitObject.h"
#include "gameFiles/entities/Convoys/convoyObject.h"
#include "gameFiles/useThroughout/stats/unitStats.h"
#include "gameFiles/useThroughout/stats/convoyStats.h"

#include "gameFiles/UI/resourceBoxes/resourceBox.h"

#include "gameFiles/components/renderComponent.h"
#include "gameFiles/components/buttonComponent.h"
#include "gameFiles/components/movementComponent.h"
#include "gameFiles/components/attackComponent.h"

class textInput;
class LevelManager;

class unitMaker : public ResourceBox {
public:
	unitMaker(SDL_FRect rSize, LevelManager* lManager, DepotObj* gameDepot) : ResourceBox(lManager, rSize), depot(gameDepot) {
		//add make unit button
		elements.push_back(new newUnitButton({ size.x + 245.0f * camera.xScale, size.y + 788.0f * camera.yScale, 170.0f * camera.xScale , 65.0f * camera.yScale }, true));
		//add cancel button
		elements.push_back(new newUnitButton({ size.x + 585.0f * camera.xScale, size.y + 788.0f * camera.yScale, 170.0f * camera.xScale , 65.0f * camera.yScale }, false));
	}
	~unitMaker() {}	

	virtual void newUnitButtonClicked(UIElement* elem) { 
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
		auto rComp = depot->getComponent<resourceComponent>();
		vector<int> counts = rComp->getAllResourceCount();
		if (checkAmounts(counts, amounts)) {
			if (elem->update(NULL)) { // make new unit
				makeUnit(amounts);
			}
			else { // make new convoy
				makeConvoy(amounts);
			}
			// Remove amount values from the depot
			for (int i = 0; i < counts.size(); i++) {
				rComp->setResourcesCount(i, counts[i] - amounts[i]);
			}
			unPause();
			//should remove box on beginning of transfer
			toDelete = true;
			depot->onClick();
		}
	}

	bool checkAmounts(vector<int> counts, vector<int> amounts) {
		for (int i = 0; i < counts.size(); i++) {
			if (counts[i] - amounts[i] < 0) {
				return false;
			}
		}
		return true;
	}

	void makeUnit(vector<int> amounts)
	{
		SDL_FRect pos = depot->getDimensions();
		//Spawn just right of the depot in the center
		pos.x = pos.x + pos.w/2 - (30 / 1440.0f * camera.dimen.h);
		pos.y = pos.y + pos.h;
		UnitObj* unit = new UnitObj(pos.x, pos.y, unitStats.width, unitStats.height, 100, manager->getNextID());
		addUnitComponents(unit, amounts);
		manager->addUnitConvoy(unit);
	}

	void addUnitComponents(UnitObj* unit, vector<int> amounts) {
		unitStats.rCount[PERSONNEL] = amounts[PERSONNEL];
		unitStats.rCount[AMMUNITION] = amounts[AMMUNITION];
		unitStats.rCount[DOS] = amounts[DOS];
		unitStats.rCount[FUEL] = amounts[FUEL];
		unitStats.rCount[SCRAP] = amounts[SCRAP];
		unitStats.addComponents(unit);
	}

	void makeConvoy(vector<int> amounts)
	{
		SDL_FRect pos = depot->getDimensions();
		//Spawn just above depot in the center
		pos.x = pos.x + pos.w;
		pos.y = pos.y + pos.h / 2;
		ConvoyObj* convoy = new ConvoyObj(pos.x, pos.y, convoyStats.width, convoyStats.height, 100, manager->getNextID());
		addConvoyComponents(convoy, amounts);
		manager->addUnitConvoy(convoy);
	}

	void addConvoyComponents(ConvoyObj* convoy, vector<int> amounts) {
		convoyStats.rCount[PERSONNEL] = amounts[PERSONNEL];
		convoyStats.rCount[AMMUNITION] = amounts[AMMUNITION];
		convoyStats.rCount[DOS] = amounts[DOS];
		convoyStats.rCount[FUEL] = amounts[FUEL];
		convoyStats.rCount[SCRAP] = amounts[SCRAP];
		convoyStats.addComponents(convoy);
	}

private:
	DepotObj* depot;
};