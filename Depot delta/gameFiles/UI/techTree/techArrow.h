#pragma once
#include "gameFiles/UI/UIElement.h"

class TechArrow : public UIElement {
public:
	TechArrow(SDL_FRect size, bool inc): UIElement(size), increase(inc){
		// Load resource hover texture
		if(increase){ texture = loadTexture("art/UI/level/arrow.png"); }
		else { texture = loadTexture("art/UI/level/flippedArrow.png"); }
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