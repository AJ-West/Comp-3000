#pragma once
#include "gameFiles/UI/resourceBoxes/resourceBox.h"
#include "gameFiles/levelHandling/levelManager.h"


ResourceBox::ResourceBox(LevelManager* lManager, SDL_FRect rSize) : UIElement(rSize), manager(lManager) {
	//Load transfer box texture - needs replacing in the future with specific unit make box
	SDL_Surface* surface = IMG_Load("art/UI/level/clipboard.png");
	if (!surface) {
		cerr << "Unable to load image! IMG_Error: " << SDL_GetError() << endl;
		return;
	}
	texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_SetTextureScaleMode(texture, SDL_SCALEMODE_PIXELART);
	SDL_DestroySurface(surface); // Free the surface after creating the texture
	if (!texture) {
		cerr << "Unable to create texture! SDL_Error: " << SDL_GetError() << endl;
		return;
	}
	addTextInputs();
}

void ResourceBox::addTextInputs() {
	//Personnel count
	elements.push_back(new textInput({ size.x + 478.0f * camera.xScale, size.y + 516.0f * camera.yScale, 60.0f * camera.xScale , 25.0f * camera.yScale }, new intRestriction()));
	//Ammo count
	elements.push_back(new textInput({ size.x + 478.0f * camera.xScale, size.y + 566.0f * camera.yScale, 60.0f * camera.xScale , 25.0f * camera.yScale }, new intRestriction()));
	//DoS count
	elements.push_back(new textInput({ size.x + 478.0f * camera.xScale, size.y + 616.0f * camera.yScale, 60.0f * camera.xScale , 25.0f * camera.yScale }, new intRestriction()));
	//Fuel count
	elements.push_back(new textInput({ size.x + 478.0f * camera.xScale, size.y + 666.0f * camera.yScale, 60.0f * camera.xScale , 25.0f * camera.yScale }, new intRestriction()));
	//Scrap count
	elements.push_back(new textInput({ size.x + 478.0f * camera.xScale, size.y + 715.0f * camera.yScale, 60.0f * camera.xScale , 25.0f * camera.yScale }, new intRestriction()));
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