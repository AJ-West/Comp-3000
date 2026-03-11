#pragma once
#include "gameFiles/UI/resourceBoxes/resourceBox.h"
#include "gameFiles/levelHandling/levelManager.h"
#include "gameFiles/UI/techTree/techArrow.h"

ResourceBox::ResourceBox(LevelManager* lManager, SDL_FRect rSize) : UIElement(rSize), manager(lManager) {
	//Load transfer box texture - needs replacing in the future with specific unit make box
	texture = loadTexture("art/UI/level/clipboard.png");
}

bool ResourceBox::update(SDL_Keycode key) {
	if (selectedElement) {
		selectedElement->update(key);
	}
	return false;
}

void ResourceBox::render(SDL_Renderer* renderer) {
	SDL_RenderTexture(renderer, texture, NULL, &size);
	SDL_SetRenderDrawColor(renderer, 163, 149, 123, 255);
	for (auto elem : elements) {
		elem->render(renderer);
	}
	if (selectedElement) {	selectedElement->toggleIndicator();	}
}

bool ResourceBox::findClickedElement(float cx, float cy) {
	for (auto elem : elements) {
		if (elem->checkClick(cx, cy)) {
			if (auto e = dynamic_cast<transferDirectionButton*>(elem)) { 
				transferDirectionButtonClicked(elem);	
			}
			else if (auto e = dynamic_cast<newUnitButton*>(elem)) {
				newUnitButtonClicked(elem);
			}
			else if (auto e = dynamic_cast<TechArrow*>(elem)) {
				selectedElement = elem;
				arrowClicked(dynamic_cast<TechArrow*>(elem));
			}
			else {
				selectedElement = elem;
			}
			return true;
		}
	}
	selectedElement = nullptr;
	return false;
}

void ResourceBox::unPause() { manager->setPaused(false); }