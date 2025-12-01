#pragma once

#include "UIElement.h"

class transferDirectionButton : public UIElement {
public:
	transferDirectionButton(SDL_FRect rSize, bool tToUnit) : UIElement(rSize), toUnit(tToUnit){
		if (toUnit) {
			text = "Convoy -> Unit";
			size = { 464.0f / 1440.0f * camera.dimen.w, 500.0f / 960.0f * camera.dimen.h, 192.0f / 1440.0f * camera.dimen.w, 48.0f / 960.0f * camera.dimen.h };
		}
		else {
			text = "Unit -> Convoy";
			size = { 784.0f / 1440.0f * camera.dimen.w, 500.0f / 960.0f * camera.dimen.h, 192.0f / 1440.0f * camera.dimen.w, 48.0f / 960.0f * camera.dimen.h };
		}
		SDL_Surface* surface = TTF_RenderText_Solid(font, text.c_str(), text.length(), { 0,0,0,255 });
		texture = SDL_CreateTextureFromSurface(renderer, surface);
	}
	~transferDirectionButton() {}

	virtual bool update(SDL_Keycode key) { // keycode will be null as it has just been clicked on
		return toUnit;
	}

	virtual void render(SDL_Renderer* renderer) {
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		SDL_RenderFillRect(renderer, &size);
		SDL_RenderTexture(renderer, texture, NULL, &size);
	}

	bool getToUnit() { return toUnit; }

private:
	bool toUnit;
	string text;
	SDL_FRect size;
	SDL_Texture* texture;
};