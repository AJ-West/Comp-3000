#pragma once
#include <iostream>

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>

#include "gameFiles/useThroughout/variables.h"
#include "gameFiles/UI/UIElement.h"

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
		SDL_RenderFillRect(renderer, &size);
		SDL_Surface* surface = TTF_RenderText_Solid(font, text.c_str(), text.length(), { 0,0,0,255 });
		SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
		SDL_RenderTexture(renderer, texture, NULL, &size);
	}

	virtual void toggleIndicator() {
		Uint32 currentTime = SDL_GetTicks();
		if (currentTime - lastIndicatorTime >= indicatorCooldown) {
			lastIndicatorTime = currentTime;
			indicator = !indicator;
			if (indicator) {
				text = text + "|";
			}
			else {
				if (!text.empty()) {
					text.pop_back();
				}
			}
		}
	}

	bool update(SDL_Keycode key) {
		if (!restrict->checkRestriction(key)) { return false; }
		if (indicator) { 
			if (!text.empty()) {
				text.pop_back();
			}
		}
		if (key == SDLK_BACKSPACE) {
			if (!text.empty()) { text.pop_back(); }
			return false;
		}
		text = text + SDL_GetKeyName(key);
		if (indicator) { text = text + '|'; }
		return false;
	}

	//getters
	string getText() { return text; }

private:
	bool indicator = false;
	int indicatorCooldown = 600;
	Uint32 lastIndicatorTime = 0;

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