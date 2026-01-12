#pragma once
#include <iostream>

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>

#include "variables.h"
#include "gameObject.h"
#include "textInput.h"
#include "transferDirectionButton.h"
#include "newUnitButton.h"

#include "resourceTransferComponent.h"

class textInput;

class ResourceBox : public UIElement {
public:
	ResourceBox(SDL_FRect rSize) : UIElement(rSize) {
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
	~ResourceBox() {}

	bool update(SDL_Keycode key) {
		if (selectedElement) {
			selectedElement->update(key);
		}
		return false;
	}

	void render(SDL_Renderer* renderer) {
		SDL_SetRenderDrawColor(renderer, 81, 72, 65, 255);
		SDL_RenderFillRect(renderer, &size);
		SDL_RenderTexture(renderer, texture, NULL, &size);
		for (auto elem : elements) {
			elem->render(renderer);
		}
	}

	bool findClickedElement(float cx, float cy) {
		for (auto elem : elements) {
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

	virtual void transferDirectionButtonClicked(UIElement* elem) { cout << "transfer"; }
	virtual void newUnitButtonClicked(UIElement* elem) { cout << "new unit"; }

	//getters
	bool getToDelete() { return toDelete; }

protected:
	vector<UIElement*> elements;
	bool toDelete = false;
private:
	SDL_Texture* texture = nullptr;

	SDL_FRect input{ camera.dimen.x + camera.dimen.w / 2 - camera.dimen.w / 20, camera.dimen.y + camera.dimen.h / 2 - camera.dimen.h / 20 , camera.dimen.w / 10 , camera.dimen.h / 10 };

	string text = "";	

	UIElement* selectedElement = nullptr;	
};