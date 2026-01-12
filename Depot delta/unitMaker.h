#pragma once

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
	unitMaker(SDL_FRect rSize, vector<UnitObj*>* gameUnits, vector<ConvoyObj*>* gameConvoys, DepotObj* gameDepot) : ResourceBox(rSize), units(gameUnits), convoys(gameConvoys), depot(gameDepot) {
		//add make unit button
		elements.push_back(new newUnitButton({ size.x + 74.0f / 1440.0f * camera.dimen.w, size.y + 140.0f / 960.0f * camera.dimen.h, 192.0f / 1440.0f * camera.dimen.w , 48.0f / 960.0f * camera.dimen.h }, true));
		//add cancel button
		elements.push_back(new newUnitButton({ size.x + 394.0f / 1440.0f * camera.dimen.w, size.y + 140.0f / 960.0f * camera.dimen.h, 192.0f / 1440.0f * camera.dimen.w , 48.0f / 960.0f * camera.dimen.h }, false));
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
		if (elem->update(NULL)) { // make new unit
			makeUnit(amounts);
		}
		else { // make new convoy
			makeConvoy(amounts);
		}
		//should remove transfer box on beginning of transfer
		toDelete = true;
	}

	void makeUnit(vector<int> amounts)
	{
		SDL_FRect pos = depot->getDimensions();
		//Spawn just above depot in the center
		pos.x = pos.x + pos.w;
		pos.y = pos.y + 25;
		UnitObj* unit = new UnitObj(pos.x, pos.y, 1, 1, 100, units->size());
		addUnitComponents(unit, amounts);
		units->emplace_back(unit);
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
		pos.y = pos.y + 25;
		ConvoyObj* convoy = new ConvoyObj(pos.x, pos.y, 1, 1, 100, convoys->size());
		addConvoyComponents(convoy, amounts);
		convoys->emplace_back(convoy);
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
	vector<UnitObj*>* units;
	vector<ConvoyObj*>* convoys;

};