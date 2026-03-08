#pragma once
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <iostream>
#include <cstdlib>
#include <vector>

#include "gameFiles/levelHandling/levelManager.h"
#include "gameFiles/entities/Depot/depotObject.h"
#include "gameFiles/UI/techTree/tech.h"
#include "gameFiles/UI/techTree/techArrow.h"

using namespace std;

enum currentTech {
	depotT,
	unitT,
	convoyT,
	sizeOfT
};

class TechTree : public UIElement {
public:
	TechTree(SDL_Renderer* renderer, LevelManager* lManager, DepotObj* depObj);
	~TechTree();

	void updateDepotScrap();

	void readTechFile();

	void loadUpgrades(XMLElement* layer, vector<Tech*>* list);

	void updateAffordable();

	void setAffordable(Tech* tech, int cost);

	void setValues(XMLElement* entity, Tech* tech, int cost);

	void saveTechFile();

	void saveUpgrades(XMLElement* layer, vector<Tech*>* list);

	void render(SDL_Renderer* renderer);

	void renderText();

	void checkHover(vector<Tech*> currentTech, SDL_Renderer* renderer);

	bool findClickedElement(float cx, float cy);

	void nextArrowClick();

	void prevArrowClick();

	void unPause();

private:
	int current = depotT;
	vector<string> currentStrings{ "Depot Technology", "Unit Technology", "Convoy Technology" };
	SDL_FRect tSize;

	SDL_Texture* background;
	SDL_Texture* Icons;

	TechArrow* leftArrow;
	TechArrow* rightArrow;

	vector<Tech*> depotTechs;
	vector<Tech*> unitTechs;
	vector<Tech*> convoyTechs;

	DepotObj* depot;

	SDL_Texture* scrap;
	SDL_FRect scrapSize;
	SDL_Texture* depotScrap;
	SDL_FRect depotScrapSize;

	LevelManager* manager;
};