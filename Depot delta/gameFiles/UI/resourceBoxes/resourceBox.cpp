#pragma once
#include "gameFiles/UI/resourceBoxes/resourceBox.h"
#include "gameFiles/levelHandling/levelManager.h"


ResourceBox::ResourceBox(LevelManager* lManager, SDL_FRect rSize) : UIElement(rSize), manager(lManager) {
	//Load transfer box texture - needs replacing in the future with specific unit make box
	SDL_Surface* surface = IMG_Load("art/UI/level/transfer.png");
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
	//Personnel count
	elements.push_back(new textInput({ size.x + 74.0f * camera.xScale, size.y + 28.0f * camera.yScale, 64.0f * camera.xScale , 64.0f * camera.yScale }, new intRestriction()));
	//Ammo count
	elements.push_back(new textInput({ size.x + 202.0f * camera.xScale, size.y + 28.0f * camera.yScale, 64.0f * camera.xScale , 64.0f * camera.yScale }, new intRestriction()));
	//DoS count
	elements.push_back(new textInput({ size.x + 330.0f * camera.xScale, size.y + 28.0f * camera.yScale, 64.0f * camera.xScale , 64.0f * camera.yScale }, new intRestriction()));
	//Fuel count
	elements.push_back(new textInput({ size.x + 458.0f * camera.xScale, size.y + 28.0f * camera.yScale, 64.0f * camera.xScale , 64.0f * camera.yScale }, new intRestriction()));
	//Scrap count
	elements.push_back(new textInput({ size.x + 586.0f * camera.xScale, size.y + 28.0f * camera.yScale, 64.0f * camera.xScale , 64.0f * camera.yScale }, new intRestriction()));
}

bool ResourceBox::update(SDL_Keycode key) {
	if (selectedElement) {
		selectedElement->update(key);
	}
	return false;
}

void ResourceBox::render(SDL_Renderer* renderer) {
	SDL_SetRenderDrawColor(renderer, 81, 72, 65, 255);
	SDL_RenderFillRect(renderer, &size);
	SDL_RenderTexture(renderer, texture, NULL, &size);
	for (auto elem : elements) {
		elem->render(renderer);
	}
}

bool ResourceBox::findClickedElement(float cx, float cy) {
	for (auto elem : elements) {
		SDL_FRect size = elem->getSize();
		cout << 'x: ' << size.x << '\n';
		cout << 'y: ' << size.y << '\n';
		cout << 'w: ' << size.w << '\n';
		cout << 'h: ' << size.h << '\n';
		if (elem->checkClick(cx, cy)) {
			if (auto e = dynamic_cast<textInput*>(elem)) { 
				selectedElement = elem; 
			}
			else if (auto e = dynamic_cast<transferDirectionButton*>(elem)) { 
				transferDirectionButtonClicked(elem);	
			}
			else if (auto e = dynamic_cast<newUnitButton*>(elem)) {
				newUnitButtonClicked(elem);
			}
			return true;
		}
	}
	selectedElement = nullptr;
	return false;
}

void ResourceBox::unPause() { manager->setPaused(false); }