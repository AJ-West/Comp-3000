#pragma once
#include <iostream>

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>

#include "variables.h"

class transferBox {
public:
	transferBox(){}
	~transferBox(){}

	bool checkClick(float x, float y) {
		if (x < input.x) return false;
		if (x > input.x + input.w) return false;
		if (y < input.y) return false;
		if (y > input.y + input.h) return false;
		return true;
	}

	void render(SDL_Renderer* renderer){
		SDL_SetRenderDrawColor(renderer, 81, 72, 65, 255);
		SDL_RenderFillRect(renderer, &outline);
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		SDL_RenderFillRect(renderer, &input);
		SDL_Surface* surface = TTF_RenderText_Solid(font, text.c_str(), text.length(), { 0,0,0,255 });
		SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
		SDL_RenderTexture(renderer, texture, NULL, &input);
	}

	void updateText(string num) {
		if (num == "b") {
			if (!text.empty()) { text.pop_back(); }
			return;
		}
		text = text + num;
	}

	void check_input(){}

private:
	SDL_FRect outline{camera.dimen.x + camera.dimen.w/2 - camera.dimen.w/10, camera.dimen.y + camera.dimen.h / 2 - camera.dimen.h / 10 , camera.dimen.w / 5 , camera.dimen.h / 5 };
	SDL_FRect input{ camera.dimen.x + camera.dimen.w / 2 - camera.dimen.w / 20, camera.dimen.y + camera.dimen.h / 2 - camera.dimen.h / 20 , camera.dimen.w / 10 , camera.dimen.h / 10 };

	string text = "";
};