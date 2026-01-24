#pragma once
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <iostream>
#include <cstdlib>

#include "gameFiles/entities/Depot/depotObject.h"
#include "gameFiles/UI/techTree/tech.h"
#include "gameFiles/UI/techTree/techArrow.h"
#include "vector"

using namespace std;

enum currentTech {
	depotT,
	unitT,
	convoyT,
	sizeOfT
};

class TechTree : public UIElement {
public:
	TechTree(SDL_Renderer* renderer, LevelManager* lManager, DepotObj* depObj): UIElement(camera.dimen), manager(lManager), depot(depObj) {
		readTechFile();
		// Load resource hover texture
		SDL_Surface* surface = IMG_Load("techTree/art/blueprint.png");
		if (!surface) {
			cerr << "Unable to load image! IMG_Error: " << SDL_GetError() << endl;
			return;
		}
		background = SDL_CreateTextureFromSurface(renderer, surface);
		//SDL_SetTextureScaleMode(background, SDL_SCALEMODE_PIXELART);
		SDL_DestroySurface(surface); // Free the surface after creating the texture
		if (!background) {
			cerr << "Unable to create texture! SDL_Error: " << SDL_GetError() << endl;
			return;
		}

		//define text box size
		tSize = { size.x+size.w/4, size.y, size.w/2, size.h/10 };

		leftArrow = new TechArrow({ size.x + size.w / 20, size.y + size.h / 2 - size.h / 20, size.w / 20, size.h / 10 }, false);
		rightArrow = new TechArrow({ size.x + size.w - size.w / 10, size.y + size.h / 2 - size.h / 20, size.w / 20, size.h / 10 }, true);
	}
	~TechTree(){
		saveTechFile();
	}

	void readTechFile() {
		XMLDocument doc;
		doc.LoadFile("techTree/currentTree.xml");
		XMLElement* root = doc.RootElement();
		XMLElement* layer = root->FirstChildElement("layers");
		while (layer) {
			string layerName = string(layer->FirstChildElement("name")->GetText());
			if (layerName == "depotUpgrades") {
				loadUpgrades(layer, &depotTechs);
			}
			else if (layerName == "unitUpgrades") {
				loadUpgrades(layer, &unitTechs);
			}
			else if (layerName == "convoyUpgrades") {
				loadUpgrades(layer, &convoyTechs);
			}
			layer = layer->NextSiblingElement("layers");
		}
	}

	void loadUpgrades(XMLElement* layer, vector<Tech*>* list) {
		XMLElement* entity = layer->FirstChildElement("entities");
		int i = 0;
		SDL_FRect pos{ 0,0,0,0 };
		while (entity) {
			div_t divV = div(i, 5);
			pos.w = size.w / 24;
			pos.h = pos.w;
			pos.x = size.w / 7 * (divV.rem + 1) + size.w / 14 - pos.w / 2;
			pos.y = size.h / 3 * divV.quot + size.h / 6 - pos.h / 2;
			i++;

			string name = string(entity->FirstChildElement("name")->GetText());
			//string keyName = string(entity->FirstChildElement("keyName")->GetText());
			int cost = atoi(entity->FirstChildElement("cost")->GetText());
			//Tech* newTech = new Tech(cost, pos, name, keyName);
			Tech* newTech = new Tech(cost, pos, name, "example description");
			newTech->setStatus(atoi(entity->FirstChildElement("status")->GetText()));
			if (newTech->getStatus() == unlocked) {
				setAffordable(newTech, cost);
			}
			list->emplace_back(newTech);
			entity = entity->NextSiblingElement("entities");
		}
	}

	void setAffordable(Tech* tech, int cost) {
		if (cost < depot->getComponent<resourceComponent>()->getResourcesCount(SCRAP)) {
			tech->setStatus(affordable);
		}
		else {
			tech->setStatus(unaffordable);
		}
	}

	void saveTechFile() {

	}

	void render(SDL_Renderer* renderer) {
		SDL_RenderTexture(renderer, background, NULL, &size); // render background over whole screen
		switch (current) {
		case depotT:
			for (auto techBox : depotTechs) {
				techBox->render(renderer);
			}
			checkHover(depotTechs, renderer);
			break;
		case unitT:
			for (auto techBox : unitTechs) {
				techBox->render(renderer);
			}
			checkHover(unitTechs, renderer);
			break;
		case convoyT:
			for (auto techBox : convoyTechs) {
				techBox->render(renderer);
			}
			checkHover(convoyTechs, renderer);
			break;
		}
		renderText();

		leftArrow->render(renderer);
		rightArrow->render(renderer);
	}

	void renderText() {
		string text = currentStrings[current];
		SDL_Surface* surface = TTF_RenderText_Solid(font, text.c_str(), text.length(), { 0,0,0,255 });
		SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
		SDL_RenderTexture(renderer, texture, NULL, &tSize);
	}

	void checkHover(vector<Tech*> currentTech, SDL_Renderer* renderer) {
		float mx, my;
		getUIScaledMousePos(&mx, &my);
		for (auto tech : currentTech) {
			if (tech->checkClick(mx, my)) {
				tech->renderHover(renderer);
			}
		}
	}

	bool findClickedElement(float cx, float cy) {
		switch (current) {
		case depotT:
			for (auto techBox : depotTechs) {
				if (techBox->checkClick(cx, cy)) {
					return true;
				}
			}
			break;
		case unitT:
			for (auto techBox : unitTechs) {
				if (techBox->checkClick(cx, cy)) {
					return true;
				}
			}
			break;
		case convoyT:
			for (auto techBox : convoyTechs) {
				if (techBox->checkClick(cx, cy)) {
					return true;
				}
			}
			break;
		}
		if (leftArrow->checkClick(cx, cy)) { prevArrowClick(); }
		if (rightArrow->checkClick(cx, cy)) { nextArrowClick(); }
		return false;
	}

	void nextArrowClick() {
		current += 1;
		if (current >= sizeOfT) {
			current = 0;
		}
	}

	void prevArrowClick() {
		current -= 1;
		if (current < 0) {
			current = sizeOfT-1;
		}
	}

	void unPause() { manager->setPaused(false); }

private:
	int current = depotT;
	vector<string> currentStrings{ "Depot Technology", "Unit Technology", "Convoy Technology" };
	SDL_FRect tSize;

	SDL_Texture* background;

	TechArrow* leftArrow;
	TechArrow* rightArrow;

	vector<Tech*> depotTechs;
	vector<Tech*> unitTechs;
	vector<Tech*> convoyTechs;

	DepotObj* depot;

	LevelManager* manager;
};