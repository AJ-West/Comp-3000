#pragma once
#include "gameFiles/levelHandling/levelManager.h"

class TutorialScreen {
public:
	TutorialScreen(LevelManager* lManager): manager(lManager) {}
	~TutorialScreen() {}

	virtual vector<string> getTexts() = 0;
	virtual vector<SDL_FRect> getTextPositions() = 0;
	virtual vector<SDL_FRect> getTargetPositions() = 0;

	virtual TutorialScreen* nextTutorial() { return nullptr; }

	void setTargetPos(vector<SDL_FRect> positions) { targetPos = positions; }

protected:
	vector<SDL_FRect> targetPos;

	LevelManager* manager;
};


class TutorialManager {
public:
	TutorialManager() {}
	~TutorialManager() {}

	void generateText(vector<string> newTexts) {
		currentTexts.clear();
		for (string text : newTexts) {
			SDL_Surface* surface = TTF_RenderText_Solid(font, text.c_str(), text.length(), { 0,0,0,255 });
			SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
			currentTexts.emplace_back(texture);
		}
	}

	void generateArrows(vector<SDL_FRect> targets) {
		currentArrows.clear();
		for (int i = 0; i < currentTextPos.size(); i++) {
			Vec2 centerText{ currentTextPos[i].x + currentTextPos[i].w/2, currentTextPos[i].y + currentTextPos[i].h / 2 };// center point of the text
			Vec2 targetCPoint = calculateClosestPoint(centerText, targets[i]); // get closet point on target object to the text
			Vec2 textCPoint = calculateClosestPoint(targetCPoint, currentTextPos[i]);
			currentArrows.emplace_back(textCPoint);
			currentArrows.emplace_back(targetCPoint);
		}
	}

	void render(SDL_Renderer* renderer) {
		if (screen) {
			renderText(renderer);
			renderArrows(renderer);
		}
	}

	void renderText(SDL_Renderer* renderer) {
		for (int i = 0; i < currentTexts.size(); i++) {
			SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // white background for text
			SDL_RenderFillRect(renderer, &currentTextPos[i]);
			SDL_RenderTexture(renderer, currentTexts[i], NULL, &currentTextPos[i]);
		}
	}

	void renderArrows(SDL_Renderer* renderer) {
		for (int i = 0; i < currentArrows.size()-1; i+=2) {
			SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255); // blue for arrow
			SDL_RenderLine(renderer, currentArrows[i].x, currentArrows[i].y, currentArrows[i+1].x, currentArrows[i+1].y);
		}
	}

	Vec2 calculateClosestPoint(Vec2 origin, SDL_FRect target) {
		vector<Vec2> options{ {target.x, target.y}, {target.x, target.y + target.h/2} ,{target.x, target.y + target.h}, 
							  {target.x + target.w/2, target.y}, {target.x + target.w / 2, target.y + target.h}, // ignore center point of object
							  {target.x + target.w, target.y}, {target.x + target.w, target.y + target.h / 2} ,{target.x + target.w, target.y + target.h} };
		float sDist = 100000000; // distance will definitely less than this
		Vec2 sVec{ 0,0 };
		for (Vec2 opt : options) {
			float dist = getDistance(origin, opt);
			if (dist <= sDist) {
				sVec = opt;
				sDist = dist;
			}
		}
		return sVec;
	}

	bool getNextTutorial() {
		changeScene(screen->nextTutorial());
		if (screen) { return true; }
		return false;
	}

	void changeScene(TutorialScreen* newScreen) {
		screen = newScreen;

		if (screen) {
			currentTextPos = screen->getTextPositions();
			generateText(screen->getTexts());
			generateArrows(screen->getTargetPositions());
		}
	}

private:
	TutorialScreen* screen = nullptr;

	vector<SDL_Texture*> currentTexts;
	vector<SDL_FRect> currentTextPos;
	vector<SDL_FRect> currentTargetPos;
	vector<Vec2> currentArrows;	
};

class DepotPointScreen: public TutorialScreen {
public:
	DepotPointScreen(LevelManager* lManager): TutorialScreen(lManager) {
		setUpPositions();
	}
	~DepotPointScreen() {}

	virtual void setUpPositions() {
		// text 0
		vector<shared_ptr<ZombieObj>> zombieList = *manager->getZombieList();
		SDL_FRect target = zombieList[0]->getDimensions();
		target.x = target.x - camera.dimen.x;
		target.y = target.y - camera.dimen.y;
		targetPositions.emplace_back(target);
		Vec2 targetCenter = { target.x + target.w / 2, target.y + target.h / 2 };
		SDL_FRect textPos{ targetCenter.x, target.y - (2 * camera.dimen.h / 12), camera.dimen.w / 6 * scaleText(texts[0]), camera.dimen.h / 12 };
		textPos.x -= textPos.w/2;
		textPositions.emplace_back(textPos);

		// text 1
		DepotObj depot = *manager->getDepot();
		target = depot.getDimensions();
		target.x = target.x - camera.dimen.x;
		target.y = target.y - camera.dimen.y;
		targetPositions.emplace_back(target);
		targetCenter = { target.x + target.w / 2, target.y + target.h / 2 };
		//textPos = { targetCenter.x, targetCenter.y, camera.dimen.w / 4 * camera.xScale * scaleText(texts[0]), camera.dimen.h / 3 * camera.yScale };
		textPos = { targetCenter.x - (camera.dimen.w / 8 * scaleText(texts[1])), target.y + target.h + (camera.dimen.h / 16), camera.dimen.w / 4 * scaleText(texts[1]), camera.dimen.h / 12 };
		textPositions.emplace_back(textPos);
	}

	virtual vector<string> getTexts() { return texts; };
	virtual vector<SDL_FRect> getTextPositions() { return textPositions; };
	virtual vector<SDL_FRect> getTargetPositions() { return targetPositions; };

private:
	vector<string> texts{"A zombie is approaching the Depot", "Click on the depot to create a unit to kill the zombie"};
	vector<SDL_FRect> textPositions;
	vector<SDL_FRect> targetPositions;
};

class ResourcePointScreen : public TutorialScreen {
public:
	ResourcePointScreen(LevelManager* lManager) : TutorialScreen(lManager) {
		setUpPositions();
	}
	~ResourcePointScreen() {}

	virtual void setUpPositions() {
		// text 0
		SDL_FRect target = { 881.0f * camera.xScale, 511.0f * camera.yScale, 60.0f * camera.xScale , 25.0f * camera.yScale };
		targetPositions.emplace_back(target);
		Vec2 targetCenter = { target.x + target.w / 2, target.y + target.h / 2 };
		SDL_FRect textPos = { targetCenter.x - target.w, target.y - target.h/4, camera.dimen.w / 4 * scaleText(texts[0]), 1.5f*target.h};
		textPos.x -= textPos.w;
		textPositions.emplace_back(textPos);

		// text 1
		target = { 881.0f * camera.xScale, 561.0f * camera.yScale, 60.0f * camera.xScale , 25.0f * camera.yScale };
		targetPositions.emplace_back(target);
		targetCenter = { target.x + target.w / 2, target.y + target.h / 2 };
		textPos = { targetCenter.x - target.w, target.y - target.h / 4, camera.dimen.w / 4 * scaleText(texts[1]), 1.5f * target.h };
		textPos.x -= textPos.w;
		textPositions.emplace_back(textPos);

		// text 2
		target = { 881.0f * camera.xScale, 611.0f * camera.yScale, 60.0f * camera.xScale , 25.0f * camera.yScale };
		targetPositions.emplace_back(target);
		targetCenter = { target.x + target.w / 2, target.y + target.h / 2 };
		textPos = { targetCenter.x - target.w, target.y - target.h / 4, camera.dimen.w / 4 * scaleText(texts[2]), 1.5f * target.h };
		textPos.x -= textPos.w;
		textPositions.emplace_back(textPos);

		// text 3
		target = { 881.0f * camera.xScale, 666.0f * camera.yScale, 60.0f * camera.xScale , 25.0f * camera.yScale };
		targetPositions.emplace_back(target);
		targetCenter = { target.x + target.w / 2, target.y + target.h / 2 };
		textPos = { targetCenter.x - target.w, target.y - target.h / 4, camera.dimen.w / 4 * scaleText(texts[3]), 1.5f * target.h };
		textPos.x -= textPos.w;
		textPositions.emplace_back(textPos);

		// text 4
		target = { 881.0f * camera.xScale, 715.0f * camera.yScale, 60.0f * camera.xScale , 25.0f * camera.yScale };
		targetPositions.emplace_back(target);
		targetCenter = { target.x + target.w / 2, target.y + target.h / 2 };
		textPos = { targetCenter.x - target.w, target.y - target.h / 4, camera.dimen.w / 4 * scaleText(texts[4]), 1.5f * target.h };
		textPos.x -= textPos.w;
		textPositions.emplace_back(textPos);
	}

	virtual vector<string> getTexts() { return texts; };
	virtual vector<SDL_FRect> getTextPositions() { return textPositions; };
	virtual vector<SDL_FRect> getTargetPositions() { return targetPositions; };

private:
	vector<string> texts{ "Personnel - sclaes strength and resource usage", "Ammunition - reuired to attack zombies", "DoS - not implemented", "Fuel - used for faster movement", "Scrap - used for upgrades at Depot" };
	vector<SDL_FRect> textPositions;
	vector<SDL_FRect> targetPositions;
};

class UnitCreateScreen : public TutorialScreen {
public:
	UnitCreateScreen(LevelManager* lManager) : TutorialScreen(lManager) {
		setUpPositions();
	}
	~UnitCreateScreen() {}

	virtual void setUpPositions() {
		// text 0
		SDL_FRect target = { camera.dimen.w/2, camera.dimen.h/20, 0.0, 0.0f };
		targetPositions.emplace_back(target);
		Vec2 targetCenter = { target.x + target.w / 2, target.y + target.h / 2 };
		SDL_FRect textPos = { targetCenter.x, targetCenter.y + target.h, camera.dimen.w / 4 * scaleText(texts[0]), camera.dimen.h / 12 };
		textPos.x -= textPos.w / 2;
		textPositions.emplace_back(textPos);

		// text 1
		target = { 881.0f * camera.xScale, 511.0f * camera.yScale, 60.0f * camera.xScale , 25.0f * camera.yScale };
		targetPositions.emplace_back(target);
		targetCenter = { target.x + target.w / 2, target.y + target.h / 2 };
		textPos = { targetCenter.x, targetCenter.y + target.h, camera.dimen.w / 4 * scaleText(texts[1]), camera.dimen.h / 12 };
		textPos.x -= textPos.w / 2;
		textPositions.emplace_back(textPos);

		// text 2
		target = { 635.0f * camera.xScale, 808.0f * camera.yScale, 170.0f * camera.xScale, 65.0f * camera.yScale };
		targetPositions.emplace_back(target);
		targetCenter = { target.x + target.w / 2, target.y + target.h / 2 };
		textPos = { targetCenter.x, targetCenter.y + 2*target.h/3, camera.dimen.w / 4 * scaleText(texts[2]), camera.dimen.h / 12 };
		textPos.x -= textPos.w / 2;
		textPositions.emplace_back(textPos);
	}

	virtual TutorialScreen* nextTutorial() { 
		manager->setTutorialUsed(RESOURCEPOINT);
		return new ResourcePointScreen(manager);
	}

	virtual vector<string> getTexts() { return texts; };
	virtual vector<SDL_FRect> getTextPositions() { return textPositions; };
	virtual vector<SDL_FRect> getTargetPositions() { return targetPositions; };

private:
	vector<string> texts{ "Here you can find the current Depots resources which increase over time", "Enter the new units resources in these", "Click here to create your unit when ready"};
	vector<SDL_FRect> textPositions;
	vector<SDL_FRect> targetPositions;
};

class SelectingUnitScreen : public TutorialScreen {
public:
	SelectingUnitScreen(LevelManager* lManager) : TutorialScreen(lManager) {
		setUpPositions();
	}
	~SelectingUnitScreen() {}

	virtual void setUpPositions() {
		vector<shared_ptr<HumanObj>> unitconvoys = *manager->getUnitConvoys();
		// text 0
		SDL_FRect target = unitconvoys[0]->getDimensions();
		targetPositions.emplace_back(target);
		Vec2 targetCenter = { target.x + target.w / 2, target.y + target.h / 2 };
		SDL_FRect textPos = { targetCenter.x, targetCenter.y - target.h, camera.dimen.w / 4 * scaleText(texts[0]), camera.dimen.h / 12 };
		textPos.x -= textPos.w / 2;
		textPos.y -= textPos.h;
		textPositions.emplace_back(textPos);

		// text 1
		targetPositions.emplace_back(target);
		textPos = { targetCenter.x - target.h, targetCenter.y, camera.dimen.w / 4 * scaleText(texts[1]), camera.dimen.h / 12 };
		textPos.x -= textPos.w;
		textPositions.emplace_back(textPos);

		// text 2
		targetPositions.emplace_back(target);
		textPos = { targetCenter.x + target.h, targetCenter.y, camera.dimen.w / 4 * scaleText(texts[1]), camera.dimen.h / 12 };
		textPositions.emplace_back(textPos);
	}

	virtual vector<string> getTexts() { return texts; };
	virtual vector<SDL_FRect> getTextPositions() { return textPositions; };
	virtual vector<SDL_FRect> getTargetPositions() { return targetPositions; };

private:
	vector<string> texts{ "Click on the unit to move it", "Left click to move", "Right click to deselect" };
	vector<SDL_FRect> textPositions;
	vector<SDL_FRect> targetPositions;
};

class UnitNoAmmoScreen : public TutorialScreen {
public:
	UnitNoAmmoScreen(LevelManager* lManager) : TutorialScreen(lManager) {
		setUpPositions();
	}
	~UnitNoAmmoScreen() {}

	virtual void setUpPositions() {
		vector<shared_ptr<HumanObj>> unitconvoys = *manager->getUnitConvoys();
		// text 0
		SDL_FRect target = unitconvoys[0]->getDimensions();
		targetPositions.emplace_back(target);
		Vec2 targetCenter = { target.x + target.w / 2, target.y + target.h / 2 };
		SDL_FRect textPos = { targetCenter.x, targetCenter.y - target.h, camera.dimen.w / 4 * scaleText(texts[0]), camera.dimen.h / 12 };
		textPos.x -= textPos.w / 2;
		textPos.y -= textPos.h;
		textPositions.emplace_back(textPos);

		// text 1
		targetPositions.emplace_back(target);
		textPos = { targetCenter.x, targetCenter.y + target.h, camera.dimen.w / 4 * scaleText(texts[1]), camera.dimen.h / 12 };
		textPos.x -= textPos.w / 2;
		textPositions.emplace_back(textPos);
	}

	virtual vector<string> getTexts() { return texts; };
	virtual vector<SDL_FRect> getTextPositions() { return textPositions; };
	virtual vector<SDL_FRect> getTargetPositions() { return targetPositions; };

private:
	vector<string> texts{ "The unit is out of ammo and needs resupplying", "Click on the depot to make a convoy" };
	vector<SDL_FRect> textPositions;
	vector<SDL_FRect> targetPositions;
};

class CreateConvoyScreen : public TutorialScreen {
public:
	CreateConvoyScreen(LevelManager* lManager) : TutorialScreen(lManager) {
		setUpPositions();
	}
	~CreateConvoyScreen() {}

	virtual void setUpPositions() {
		// text 0
		SDL_FRect target = { 220.0f * camera.xScale, 20.0f * camera.yScale, 1000.0f * camera.xScale, 1000.0f * camera.yScale };
		target.x += target.w / 20;
		target.y += target.h / 2 - target.h / 20;
		target.w = target.w / 20;
		target.h = target.h / 10;
		targetPositions.emplace_back(target);
		Vec2 targetCenter = { target.x + target.w / 2, target.y + target.h / 2 };
		SDL_FRect textPos = { targetCenter.x + target.w, target.h, camera.dimen.w / 4 * scaleText(texts[0]), target.h };
		textPositions.emplace_back(textPos);
	}

	virtual vector<string> getTexts() { return texts; };
	virtual vector<SDL_FRect> getTextPositions() { return textPositions; };
	virtual vector<SDL_FRect> getTargetPositions() { return targetPositions; };

private:
	vector<string> texts{ "Click here to create a convoy instead of a unit" };
	vector<SDL_FRect> textPositions;
	vector<SDL_FRect> targetPositions;
};

class SelectingConvoyScreen : public TutorialScreen {
public:
	SelectingConvoyScreen(LevelManager* lManager, int cIndex) : TutorialScreen(lManager), index(cIndex) {
		setUpPositions();
	}
	~SelectingConvoyScreen() {}

	virtual void setUpPositions() {
		vector<shared_ptr<HumanObj>> unitconvoys = *manager->getUnitConvoys();
		// text 0
		SDL_FRect target = unitconvoys[index]->getDimensions();
		targetPositions.emplace_back(target);
		Vec2 targetCenter = { target.x + target.w / 2, target.y + target.h / 2 };
		SDL_FRect textPos = { targetCenter.x, targetCenter.y - target.h, camera.dimen.w / 4 * scaleText(texts[0]), camera.dimen.h / 12 };
		textPos.x -= textPos.w / 2;
		textPos.y -= textPos.h;
		textPositions.emplace_back(textPos);

		// text 1
		targetPositions.emplace_back(target);
		textPos = { targetCenter.x, targetCenter.y + target.h, camera.dimen.w / 4 * scaleText(texts[1]), camera.dimen.h / 12 };
		textPos.x -= textPos.w;
		textPositions.emplace_back(textPos);
	}

	virtual vector<string> getTexts() { return texts; };
	virtual vector<SDL_FRect> getTextPositions() { return textPositions; };
	virtual vector<SDL_FRect> getTargetPositions() { return targetPositions; };

private:
	int index = 0;
	vector<string> texts{ "Move the convoy close to the unit", "Right click to initiate transfer" };
	vector<SDL_FRect> textPositions;
	vector<SDL_FRect> targetPositions;
};

class ResourceTransferScreen : public TutorialScreen {
public:
	ResourceTransferScreen(LevelManager* lManager) : TutorialScreen(lManager) {
		setUpPositions();
	}
	~ResourceTransferScreen() {}

	virtual void setUpPositions() {
		// text 0
		SDL_FRect target = { 635.0f * camera.xScale, 808.0f * camera.yScale, 170.0f * camera.xScale, 65.0f * camera.yScale };
		targetPositions.emplace_back(target);
		Vec2 targetCenter = { target.x + target.w / 2, target.y + target.h / 2 };
		SDL_FRect textPos = { targetCenter.x, targetCenter.y + 2 * target.h / 3, camera.dimen.w / 4 * scaleText(texts[0]), camera.dimen.h / 12 };
		textPos.x -= textPos.w / 2;
		textPositions.emplace_back(textPos);
	}

	virtual TutorialScreen* nextTutorial() {
		manager->setTutorialUsed(RESOURCEPOINT);
		return new ResourcePointScreen(manager);
	}

	virtual vector<string> getTexts() { return texts; };
	virtual vector<SDL_FRect> getTextPositions() { return textPositions; };
	virtual vector<SDL_FRect> getTargetPositions() { return targetPositions; };

private:
	vector<string> texts{ "Arrows define direction of transfer" };
	vector<SDL_FRect> textPositions;
	vector<SDL_FRect> targetPositions;
};

class BuildingsScreen : public TutorialScreen {
public:
	BuildingsScreen(LevelManager* lManager) : TutorialScreen(lManager) {
		setUpPositions();
	}
	~BuildingsScreen() {}

	virtual void setUpPositions() {
		vector<shared_ptr<BuildingObj>> buildings = *manager->getBuildingList();
		// text 0
		SDL_FRect target = buildings[0]->getDimensions();
		targetPositions.emplace_back(target);
		Vec2 targetCenter = { target.x + target.w / 2, target.y + target.h / 2 };
		SDL_FRect textPos = { targetCenter.x, targetCenter.y - target.h, camera.dimen.w / 4 * scaleText(texts[0]), camera.dimen.h / 12 };
		textPos.x -= textPos.w / 2;
		textPos.y -= textPos.h;
		textPositions.emplace_back(textPos);

		// text 1
		targetPositions.emplace_back(target);
		textPos = { targetCenter.x, targetCenter.y + target.h, camera.dimen.w / 4 * scaleText(texts[1]), camera.dimen.h / 12 };
		textPos.x -= textPos.w;
		textPositions.emplace_back(textPos);
	}

	virtual TutorialScreen* nextTutorial() {
		manager->setTutorialUsed(WINCONDITION);
		return new WinConditionScreen(manager);
	}

	virtual vector<string> getTexts() { return texts; };
	virtual vector<SDL_FRect> getTextPositions() { return textPositions; };
	virtual vector<SDL_FRect> getTargetPositions() { return targetPositions; };

private:
	int index = 0;
	vector<string> texts{ "Buildings generate more resources", "Different buildings generate and store more of specific resources" };
	vector<SDL_FRect> textPositions;
	vector<SDL_FRect> targetPositions;
};

class WinConditionScreen : public TutorialScreen {
public:
	WinConditionScreen(LevelManager* lManager) : TutorialScreen(lManager) {
		setUpPositions();
	}
	~WinConditionScreen() {}

	virtual void setUpPositions() { 
		// text 0
		SDL_FRect target{camera.dimen.w/2, 2*camera.dimen.h/5, camera.dimen.w / 4 * scaleText(texts[0]), camera.dimen.h / 12 };
		target.x -= target.w / 2;
		targetPositions.emplace_back(target);
		SDL_FRect textPos = target;
		textPositions.emplace_back(textPos);

		// text 1
		target = { camera.dimen.w / 2, 3 * camera.dimen.h / 5, camera.dimen.w / 4 * scaleText(texts[0]), camera.dimen.h / 12 };
		target.x -= target.w / 2;
		targetPositions.emplace_back(target);
		textPos = target;
		textPositions.emplace_back(textPos);

		// text 2
		target = { camera.dimen.w / 2, 4 * camera.dimen.h / 5, camera.dimen.w / 4 * scaleText(texts[0]), camera.dimen.h / 12 };
		target.x -= target.w / 2;
		targetPositions.emplace_back(target);
		textPos = target;
		textPositions.emplace_back(textPos);
	}

	virtual vector<string> getTexts() { return texts; };
	virtual vector<SDL_FRect> getTextPositions() { return textPositions; };
	virtual vector<SDL_FRect> getTargetPositions() { return targetPositions; };

private:
	int index = 0;
	vector<string> texts{ "Clear the map of zombies", "Survive all waves", "Good Luck"};
	vector<SDL_FRect> textPositions;
	vector<SDL_FRect> targetPositions;
};

//to do add tutorial for tech tree