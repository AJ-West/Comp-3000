#pragma once
#include <iostream>

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>

#include "variables.h"
#include "gameObject.h"
#include "textInput.h"
#include "transferDirectionButton.h"

#include "resourceTransferComponent.h"

class textInput;

class transferBox: public UIElement {
public:
	transferBox(SDL_FRect rSize, GameObject* sUnit, GameObject* sConvoy) : UIElement(rSize), unit(sUnit), convoy(sConvoy) {
		//Load transfer box texture
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
		elements.push_back(new textInput({ size.x + 74.0f / 1440.0f * camera.dimen.w, size.y + 28.0f / 960.0f * camera.dimen.h, 64.0f / 1440.0f * camera.dimen.w , 64.0f / 960.0f * camera.dimen.h }, new intRestriction()));
		//Ammo count
		elements.push_back(new textInput({ size.x + 202.0f / 1440.0f * camera.dimen.w, size.y + 28.0f / 960.0f * camera.dimen.h, 64.0f / 1440.0f * camera.dimen.w , 64.0f / 960.0f * camera.dimen.h }, new intRestriction()));
		//DoS count
		elements.push_back(new textInput({ size.x + 330.0f / 1440.0f * camera.dimen.w, size.y + 28.0f / 960.0f * camera.dimen.h, 64.0f / 1440.0f * camera.dimen.w , 64.0f / 960.0f * camera.dimen.h }, new intRestriction()));
		//Fuel count
		elements.push_back(new textInput({ size.x + 458.0f / 1440.0f * camera.dimen.w, size.y + 28.0f / 960.0f * camera.dimen.h, 64.0f / 1440.0f * camera.dimen.w , 64.0f / 960.0f * camera.dimen.h }, new intRestriction()));
		//Scrap count
		elements.push_back(new textInput({ size.x + 586.0f / 1440.0f * camera.dimen.w, size.y + 28.0f / 960.0f * camera.dimen.h, 64.0f / 1440.0f * camera.dimen.w , 64.0f / 960.0f * camera.dimen.h }, new intRestriction()));

		//add convoy->unit button
		elements.push_back(new transferDirectionButton({ size.x + 74.0f / 1440.0f * camera.dimen.w, size.y + 140.0f / 960.0f * camera.dimen.h, 192.0f / 1440.0f * camera.dimen.w , 48.0f / 960.0f * camera.dimen.h }, true));
		//add unit->convoy button
		elements.push_back(new transferDirectionButton({ size.x + 394.0f / 1440.0f * camera.dimen.w, size.y + 140.0f / 960.0f * camera.dimen.h, 192.0f / 1440.0f * camera.dimen.w , 48.0f / 960.0f * camera.dimen.h }, false));
	}
	~transferBox(){}

	bool update(SDL_Keycode key) {
		if (selectedElement) {
			selectedElement->update(key);
		}
		return false;
	}

	void render(SDL_Renderer* renderer){
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
					if (elem->update(NULL)) {
						convoy->getComponent<resourceTransferComponent>()->initiateTransfer(unit, amounts);
					}
					else {
						unit->getComponent<resourceTransferComponent>()->initiateTransfer(convoy, amounts);
					}
					//should remove transfer box on beginning of transfer
					toDelete = true;
				}
				return true;
			}
		}
		selectedElement = nullptr;
			return false;
	}
	
	//getters
	bool getToDelete() { return toDelete; }

private:
	SDL_Texture* texture;

	SDL_FRect input{ camera.dimen.x + camera.dimen.w / 2 - camera.dimen.w / 20, camera.dimen.y + camera.dimen.h / 2 - camera.dimen.h / 20 , camera.dimen.w / 10 , camera.dimen.h / 10 };

	string text = "";

	vector<UIElement*> elements;

	UIElement* selectedElement = nullptr;

	GameObject* unit;
	GameObject* convoy;

	bool toDelete = false;
};