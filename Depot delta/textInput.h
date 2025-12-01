#pragma once
#include <iostream>

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>

#include "variables.h"
#include "UIElement.h"

class intRestriction;

class restriction {
public:
	restriction() {}
	~restriction() {}

	virtual bool checkRestriction(SDL_Keycode key) = 0;
};

class textInput: public UIElement {
public:
	textInput(SDL_FRect rSize, restriction* rest): UIElement(rSize), restrict(rest) {}
	~textInput() {}

	virtual void render(SDL_Renderer* renderer) {
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		SDL_RenderFillRect(renderer, &size);
		SDL_Surface* surface = TTF_RenderText_Solid(font, text.c_str(), text.length(), { 0,0,0,255 });
		SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
		SDL_RenderTexture(renderer, texture, NULL, &size);
	}

	void update(SDL_Keycode key) {
		if (!restrict->checkRestriction(key)) { return; }
		if (key == SDLK_BACKSPACE) {
			if (!text.empty()) { text.pop_back(); }
			return;
		}
		text = text + SDL_GetKeyName(key);
	}

private:
	restriction* restrict;

	string text;
};

class intRestriction: public restriction { // temp method will need refactoring and improving at a later date
public:
	intRestriction(){}
	~intRestriction(){}

	bool checkRestriction(SDL_Keycode key) {
		auto it = find(valid.begin(), valid.end(), key);
		if (it != valid.end()) { return true; }
		return false;
	}
private:
	vector<SDL_Keycode> valid{ SDLK_0, SDLK_1, SDLK_2, SDLK_3, SDLK_4, SDLK_5, SDLK_6, SDLK_7, SDLK_8, SDLK_9, SDLK_BACKSPACE };
};