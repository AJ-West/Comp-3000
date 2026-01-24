#pragma once
#include "gameFiles/UI/UIElement.h"

class TechArrow : public UIElement {
public:
	TechArrow(SDL_FRect size, bool inc): UIElement(size), increase(inc){
		// Load resource hover texture
		SDL_Surface* surface = IMG_Load("art/UI/level/flippedArrow.png");
		if(increase){ surface = IMG_Load("art/UI/level/arrow.png"); }
		if (!surface) {
			cerr << "Unable to load image! IMG_Error: " << SDL_GetError() << endl;
			return;
		}
		texture = SDL_CreateTextureFromSurface(renderer, surface);
		SDL_DestroySurface(surface); // Free the surface after creating the texture
		if (!texture) {
			cerr << "Unable to create texture! SDL_Error: " << SDL_GetError() << endl;
			return;
		}
	}
	~TechArrow(){}

	void render(SDL_Renderer* renderer) {
		SDL_RenderTexture(renderer, texture, NULL, &size);
	}

	bool getIncrease() { return increase; }

private:
	bool increase;

	SDL_Texture* texture;
};