#pragma once

#include "levelManager.h"

#include "depotObject.h"
#include "unitObject.h"
#include "convoyObject.h"

#include "resourceBox.h"

#include "renderComponent.h"
#include "buttonComponent.h"
#include "movementComponent.h"
#include "attackComponent.h"

class textInput;

class unitMaker : public ResourceBox {
public:
	unitMaker(SDL_FRect rSize, LevelManager* lManager, DepotObj* gameDepot) : ResourceBox(rSize), manager(lManager), depot(gameDepot) {
		//add make unit button
		elements.push_back(new newUnitButton({ size.x + 74.0f * camera.xScale, size.y + 140.0f * camera.yScale, 192.0f * camera.xScale , 48.0f * camera.yScale }, true));
		//add cancel button
		elements.push_back(new newUnitButton({ size.x + 394.0f * camera.xScale, size.y + 140.0f * camera.yScale, 192.0f * camera.xScale , 48.0f * camera.yScale }, false));
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
			//should remove box on beginning of transfer
			toDelete = true;
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
		UnitObj* unit = new UnitObj(pos.x, pos.y, 4, 4, 100, manager->getNextID());
		addUnitComponents(unit, amounts);
		manager->addUnitConvoy(unit);
	}

	void addUnitComponents(UnitObj* unit, vector<int> amounts) {
		unitStats stats;
		stats.rCount[PERSONNEL] = amounts[PERSONNEL];
		stats.rCount[AMMUNITION] = amounts[AMMUNITION];
		stats.rCount[DOS] = amounts[DOS];
		stats.rCount[FUEL] = amounts[FUEL];
		stats.rCount[SCRAP] = amounts[SCRAP];
		stats.addComponents(unit);		
	}

	void makeConvoy(vector<int> amounts)
	{
		SDL_FRect pos = depot->getDimensions();
		//Spawn just above depot in the center
		pos.x = pos.x + pos.w;
		pos.y = pos.y + pos.h / 2;
		ConvoyObj* convoy = new ConvoyObj(pos.x, pos.y, 1, 1, 100, manager->getNextID());
		addConvoyComponents(convoy, amounts);
		manager->addUnitConvoy(convoy);
	}

	void addConvoyComponents(ConvoyObj* convoy, vector<int> amounts) {
		convoyStats stats;
		stats.rCount[PERSONNEL] = amounts[PERSONNEL];
		stats.rCount[AMMUNITION] = amounts[AMMUNITION];
		stats.rCount[DOS] = amounts[DOS];
		stats.rCount[FUEL] = amounts[FUEL];
		stats.rCount[SCRAP] = amounts[SCRAP];
		stats.addComponents(convoy);
	}

private:
	DepotObj* depot;

	LevelManager* manager;
};