#pragma once

#include "gameFiles/levelHandling/levelManager.h"

#include "gameFiles/entities/Depot/depotObject.h"
#include "gameFiles/entities/Units/unitObject.h"
#include "gameFiles/entities/Convoys/convoyObject.h"
#include "gameFiles/useThroughout/stats/unitStats.h"
#include "gameFiles/useThroughout/stats/convoyStats.h"

#include "gameFiles/UI/resourceBoxes/resourceBox.h"
#include "gameFiles/UI/techTree/techArrow.h"

#include "gameFiles/components/renderComponent.h"
#include "gameFiles/components/buttonComponent.h"
#include "gameFiles/components/movementComponent.h"
#include "gameFiles/components/attackComponent.h"

#include "gameFiles/UI/resourceBoxes/makingStatsBox.h"

class textInput;
class LevelManager;

class unitMaker : public ResourceBox {
public:
	unitMaker(SDL_FRect rSize, LevelManager* lManager, DepotObj* gameDepot) : ResourceBox(lManager, rSize), depot(gameDepot) {
		texture = loadTexture("art/UI/level/newUnitBoard.png");

		addTextInputs();

		//add make unit button
		elements.push_back(new newUnitButton({ size.x + size.w/2 - (85.0f * camera.xScale), size.y + 788.0f * camera.yScale, 170.0f * camera.xScale , 65.0f * camera.yScale }, true));

		elements.push_back(new TechArrow({ size.x + size.w / 20, size.y + size.h / 2 - size.h / 20, size.w / 20, size.h / 10 }, false));
		elements.push_back(new TechArrow({ size.x + size.w - size.w / 10, size.y + size.h / 2 - size.h / 20, size.w / 20, size.h / 10 }, true));

		statsBox = new MakingStatsBox({ size.x + 269 * camera.xScale, size.y + 172 * camera.yScale, 482 * camera.xScale, 605 * camera.yScale });
	}
	~unitMaker() {}	

	virtual void addTextInputs() {
		//Personnel count
		elements.push_back(new textInput({ size.x + 661.0f * camera.xScale, size.y + 491.0f * camera.yScale, 60.0f * camera.xScale , 25.0f * camera.yScale }, new intRestriction()));
		//Ammo count
		elements.push_back(new textInput({ size.x + 661.0f * camera.xScale, size.y + 541.0f * camera.yScale, 60.0f * camera.xScale , 25.0f * camera.yScale }, new intRestriction()));
		//DoS count
		elements.push_back(new textInput({ size.x + 661.0f * camera.xScale, size.y + 591.0f * camera.yScale, 60.0f * camera.xScale , 25.0f * camera.yScale }, new intRestriction()));
		//Fuel count
		elements.push_back(new textInput({ size.x + 661.0f * camera.xScale, size.y + 646.0f * camera.yScale, 60.0f * camera.xScale , 25.0f * camera.yScale }, new intRestriction()));
		//Scrap count
		elements.push_back(new textInput({ size.x + 661.0f * camera.xScale, size.y + 695.0f * camera.yScale, 60.0f * camera.xScale , 25.0f * camera.yScale }, new intRestriction()));
	}

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
			if (typeIndex == basicUnit) { // make new unit
				makeUnit(amounts);
			}
			else if (typeIndex == basicConvoy) { // make new convoy
				makeConvoy(amounts);
			}
			// Remove amount values from the depot
			for (int i = 0; i < counts.size(); i++) {
				rComp->setResourcesCount(i, counts[i] - amounts[i]);
			}
			soundEffectEngine->play2D("soundEffects/voice acting/unitsconvoys/spawn.wav");
			unPause();
			//should remove box on beginning of transfer
			toDelete = true;
			depot->onClick();
		}

		selectedElement = elem;
	}

	virtual void arrowClicked(TechArrow* elem) {
		if (elem->getIncrease()) {
			typeIndex = unlockedUnit.getNextIndex(typeIndex);
		}
		else {
			typeIndex = unlockedUnit.getPrevIndex(typeIndex);
		}
		statsBox->updateArt(typeIndex);
	}

	virtual void render(SDL_Renderer* renderer) {
		SDL_RenderTexture(renderer, texture, NULL, &size);
		SDL_SetRenderDrawColor(renderer, 163, 149, 123, 255);
		for (auto elem : elements) {
			elem->render(renderer);
		}
		if (selectedElement) { selectedElement->toggleIndicator(); }
		statsBox->render(renderer);
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
		//Spawn just below the depot in the center
		pos.x = pos.x + pos.w/2 - (30 / 1440.0f * camera.dimen.h);
		pos.y = pos.y + pos.h;
		UnitObj* unit = new UnitObj(pos.x, pos.y, unitStats.width, unitStats.height, unitStats.maxHealth, manager->getNextID());
		addUnitComponents(unit, amounts);
		unit->updateTargets(manager->getZombieList());
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
		//Spawn just below the depot in the center
		pos.x = pos.x + pos.w / 2 - (30 / 1440.0f * camera.dimen.h);
		pos.y = pos.y + pos.h;
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

	MakingStatsBox* statsBox;

	int typeIndex = 0;
};