#pragma once

#include "gameFiles/UI/techTree/techTree.h"

TechTree::TechTree(SDL_Renderer* renderer, LevelManager* lManager, DepotObj* depObj) : UIElement({ 0, 0, camera.dimen.w, camera.dimen.h }), manager(lManager), depot(depObj) {
	// load background
	background = loadTexture("techTree/art/blueprint.png");
	// load each tech icon
	Icons = loadTexture("techTree/art/Icons.png");
	// load scrap icon
	scrap = loadTexture("techTree/art/scrap.png");

	readTechFile();
	updateAffordable();

	//define text box size
	tSize = { size.x + size.w / 4, size.y, size.w / 2, size.h / 10 };

	scrapSize = { size.x + size.w - size.w / 6 + size.w / 20, size.y, size.w / 20, size.w / 20 };
	depotScrapSize = { size.x + size.w - size.w / 6 + size.w / 10, size.y, size.w / 20, size.w / 20 };
	updateDepotScrap();

	leftArrow = new TechArrow({ size.x + size.w / 20, size.y + size.h / 2 - size.h / 20, size.w / 20, size.h / 10 }, false);
	rightArrow = new TechArrow({ size.x + size.w - size.w / 10, size.y + size.h / 2 - size.h / 20, size.w / 20, size.h / 10 }, true);
}

TechTree::~TechTree() {
	saveTechFile();
}

void TechTree::updateDepotScrap() {
	string depScrap = to_string(depot->getComponent<resourceComponent>()->getResourcesCount(SCRAP));
	SDL_Surface* surface = TTF_RenderText_Solid(font, depScrap.c_str(), depScrap.length(), { 0,0,0,255 });
	depotScrap = SDL_CreateTextureFromSurface(renderer, surface);
}

void TechTree::readTechFile() {
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

void TechTree::loadUpgrades(XMLElement* layer, vector<Tech*>* list) {
	XMLElement* entity = layer->FirstChildElement("entities");
	int i = 0;
	SDL_FRect pos{ 0,0,0,0 };
	while (entity) {
		div_t divV = div(i, 5);
		pos.w = size.w / 12;
		pos.h = pos.w;
		pos.x = size.w / 7 * (divV.rem + 1) + size.w / 14 - pos.w / 2;
		pos.y = size.h / 3 * divV.quot + size.h / 6 - pos.h / 2;
		i++;

		string name = string(entity->FirstChildElement("name")->GetText());
		string keyName = string(entity->FirstChildElement("keyName")->GetText());
		int cost = atoi(entity->FirstChildElement("cost")->GetText());
		//Tech* newTech = new Tech(cost, pos, name, keyName);
		Tech* newTech = new Tech(cost, pos, name, string(entity->FirstChildElement("description")->GetText()), keyName, atoi(entity->FirstChildElement("upgradeLocation")->GetText()), Icons);
		setValues(entity, newTech, cost);
		list->emplace_back(newTech);
		entity = entity->NextSiblingElement("entities");
	}
}

void TechTree::updateAffordable() {
	for (auto tech : depotTechs) {
		setAffordable(tech, tech->getCost());
	}
	for (auto tech : unitTechs) {
		setAffordable(tech, tech->getCost());
	}
	for (auto tech : convoyTechs) {
		setAffordable(tech, tech->getCost());
	}
}

void TechTree::setAffordable(Tech* tech, int cost) {
	if (tech->getStatus() != locked) {
		if (cost < depot->getComponent<resourceComponent>()->getResourcesCount(SCRAP)) {
			tech->setStatus(affordable);
		}
		else {
			tech->setStatus(unaffordable);
		}
	}
}

void TechTree::setValues(XMLElement* entity, Tech* tech, int cost) {
	tech->setID(atoi(entity->FirstChildElement("id")->GetText()));
	tech->setStatus(atoi(entity->FirstChildElement("status")->GetText()));
	if (tech->getStatus() == unlocked) {
		setAffordable(tech, cost);
	}
	tech->setType(atoi(entity->FirstChildElement("type")->GetText()));
	if (atoi(entity->FirstChildElement("type")->GetText()) == modifier) {
		tech->setModifyValue(atof(entity->FirstChildElement("modifyValue")->GetText()));
	}
	tech->setPurchaseAmount(atoi(entity->FirstChildElement("purchaseAmount")->GetText()));
	tech->setBoughtAmount(atoi(entity->FirstChildElement("boughtAmount")->GetText()));
	tech->calcLoc();
	tech->updateDescription();
}

void TechTree::saveTechFile() {
	XMLDocument doc;
	doc.LoadFile("techTree/currentTree.xml");
	XMLElement* root = doc.RootElement();
	XMLElement* layer = root->FirstChildElement("layers");
	while (layer) {
		string layerName = string(layer->FirstChildElement("name")->GetText());
		if (layerName == "depotUpgrades") {
			saveUpgrades(layer, &depotTechs);
		}
		else if (layerName == "unitUpgrades") {
			saveUpgrades(layer, &unitTechs);
		}
		else if (layerName == "convoyUpgrades") {
			saveUpgrades(layer, &convoyTechs);
		}
		layer = layer->NextSiblingElement("layers");
	}
	doc.SaveFile("techTree/currentTree.xml");
}

void TechTree::saveUpgrades(XMLElement* layer, vector<Tech*>* list) {
	XMLElement* entity = layer->FirstChildElement("entities");
	while (entity) {
		for (auto tech : *list) {
			if (tech->getID() == atoi(entity->FirstChildElement("id")->GetText())) {
				entity->FirstChildElement("cost")->SetText(tech->getCost());
				entity->FirstChildElement("purchaseAmount")->SetText(tech->getPA());
				entity->FirstChildElement("boughtAmount")->SetText(tech->getBA());
				entity->FirstChildElement("status")->SetText(tech->getStatus());
			}
		}
		entity = entity->NextSiblingElement("entities");
	}
}


void TechTree::render(SDL_Renderer* renderer) {
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

	SDL_RenderTexture(renderer, scrap, NULL, &scrapSize); // render scrap image
	SDL_RenderTexture(renderer, depotScrap, NULL, &depotScrapSize); // render scrap text

	leftArrow->render(renderer);
	rightArrow->render(renderer);
}

void TechTree::renderText() {
	string text = currentStrings[current];
	SDL_Surface* surface = TTF_RenderText_Solid(font, text.c_str(), text.length(), { 0,0,0,255 });
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_RenderTexture(renderer, texture, NULL, &tSize);
}

void TechTree::checkHover(vector<Tech*> currentTech, SDL_Renderer* renderer) {
	float mx, my;
	getUIScaledMousePos(&mx, &my);
	for (auto tech : currentTech) {
		if (tech->checkClick(mx, my)) {
			tech->renderHover(renderer);
		}
	}
}

bool TechTree::findClickedElement(float cx, float cy) {
	switch (current) {
	case depotT:
		for (auto techBox : depotTechs) {
			if (techBox->checkClick(cx, cy)) {
				if (techBox->buy()) {
					depot->getComponent<resourceComponent>()->decreaseResourceCount(SCRAP, techBox->getCost());
					techBox->increaseCost();
					if (techBox->getType() == modifier) {
						depot->updateStats(techBox->getKeyName());
					}
					updateDepotScrap();
					updateAffordable();
				}
				return true;
			}
		}
		break;
	case unitT:
		for (auto techBox : unitTechs) {
			if (techBox->checkClick(cx, cy)) {
				if (techBox->buy()) {
					depot->getComponent<resourceComponent>()->decreaseResourceCount(SCRAP, techBox->getCost());
					techBox->increaseCost();
					if (techBox->getType() == modifier) {
						manager->updateStats(techBox->getKeyName(), true);
					}
					updateDepotScrap();
					updateAffordable();
				}
				return true;
			}
		}
		break;
	case convoyT:
		for (auto techBox : convoyTechs) {
			if (techBox->checkClick(cx, cy)) {
				if (techBox->buy()) {
					depot->getComponent<resourceComponent>()->decreaseResourceCount(SCRAP, techBox->getCost());
					techBox->increaseCost();
					if (techBox->getType() == modifier) {
						manager->updateStats(techBox->getKeyName(), false);
					}
					updateDepotScrap();
					updateAffordable();
				}
				return true;
			}
		}
		break;
	}
	if (leftArrow->checkClick(cx, cy)) { prevArrowClick(); }
	if (rightArrow->checkClick(cx, cy)) { nextArrowClick(); }
	return false;
}

void TechTree::nextArrowClick() {
	current += 1;
	if (current >= sizeOfT) {
		current = 0;
	}
}

void TechTree::prevArrowClick() {
	current -= 1;
	if (current < 0) {
		current = sizeOfT - 1;
	}
}

void TechTree::unPause() { manager->setPaused(false); }
