#pragma once
#include <iostream>

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>

#include "variables.h"
#include "textInput.h"

class textInput;

class transferBox: public UIElement {
public:
	transferBox(SDL_FRect rSize) : UIElement(rSize) {
		elements.push_back(new textInput({ camera.dimen.x + camera.dimen.w / 2 - camera.dimen.w / 20, camera.dimen.y + camera.dimen.h / 2 - camera.dimen.h / 20 , camera.dimen.w / 10 , camera.dimen.h / 10 }, new intRestriction()));
	}
	~transferBox(){}

	void update(SDL_Keycode key) {
		if (selectedElement) {
			selectedElement->update(key);
		}
	}

	void render(SDL_Renderer* renderer){
		SDL_SetRenderDrawColor(renderer, 81, 72, 65, 255);
		SDL_RenderFillRect(renderer, &outline);
		for (auto elem : elements) {
			elem->render(renderer);
		}
	}

	bool findClickedElement(float cx, float cy) {
		for (auto elem : elements) {
			if (elem->checkClick(cx, cy)) {
				selectedElement = elem;
				return true;
			}
		}
		selectedElement = nullptr;
			return false;
	}

private:
	SDL_FRect outline{camera.dimen.x + camera.dimen.w/2 - camera.dimen.w/10, camera.dimen.y + camera.dimen.h / 2 - camera.dimen.h / 10 , camera.dimen.w / 5 , camera.dimen.h / 5 };
	SDL_FRect input{ camera.dimen.x + camera.dimen.w / 2 - camera.dimen.w / 20, camera.dimen.y + camera.dimen.h / 2 - camera.dimen.h / 20 , camera.dimen.w / 10 , camera.dimen.h / 10 };

	string text = "";

	vector<UIElement*> elements;

	UIElement* selectedElement = nullptr;
};