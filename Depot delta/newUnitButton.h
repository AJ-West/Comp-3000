#pragma once

#include "UIElement.h"

class newUnitButton : public UIElement {
public:
	newUnitButton(SDL_FRect rSize, bool makingUnit) : UIElement(rSize), makeUnit(makingUnit) {
		if (makeUnit) {
			text = "Create Unit";
			size = { 464.0f / 1440.0f * camera.dimen.w, 500.0f / 960.0f * camera.dimen.h, 192.0f / 1440.0f * camera.dimen.w, 48.0f / 960.0f * camera.dimen.h };
		}
		else {
			text = "Cancel";
			size = { 784.0f / 1440.0f * camera.dimen.w, 500.0f / 960.0f * camera.dimen.h, 192.0f / 1440.0f * camera.dimen.w, 48.0f / 960.0f * camera.dimen.h };
		}
		SDL_Surface* surface = TTF_RenderText_Solid(font, text.c_str(), text.length(), { 0,0,0,255 });
		texture = SDL_CreateTextureFromSurface(renderer, surface);
	}
	~newUnitButton() {}

	virtual bool update(SDL_Keycode key) { // keycode will be null as it has just been clicked on
		return makeUnit;
	}

	virtual void render(SDL_Renderer* renderer) {
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		SDL_RenderFillRect(renderer, &size);
		SDL_RenderTexture(renderer, texture, NULL, &size);
	}

	bool getMakeUnit() { return makeUnit; }

private:
	bool makeUnit;
	std::string text;
	SDL_FRect size;
	SDL_Texture* texture;
};