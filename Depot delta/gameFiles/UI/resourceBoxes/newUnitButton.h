#pragma once

#include "gameFiles/UI/UIElement.h"


class newUnitButton : public UIElement {
public:
	newUnitButton(SDL_FRect rSize, bool unit) : UIElement(rSize), isUnit(unit) {
		if (isUnit) {
			text = "Create Unit";
		}
		else {
			text = "Create Convoy";
		}
		SDL_Surface* surface = TTF_RenderText_Solid(font, text.c_str(), text.length(), { 0,0,0,255 });
		texture = SDL_CreateTextureFromSurface(renderer, surface);
	}
	~newUnitButton() {}

	virtual bool update(SDL_Keycode key) { // keycode will be null as it has just been clicked on
		return isUnit;
	}

	virtual void render(SDL_Renderer* renderer) {
		SDL_SetRenderDrawColor(renderer, 163, 149, 123, 255);
		SDL_RenderFillRect(renderer, &size);
		SDL_RenderTexture(renderer, texture, NULL, &size);
	}

	bool getIsUnit() { return isUnit; }

private:
	bool isUnit;
	std::string text;
	SDL_Texture* texture;
};