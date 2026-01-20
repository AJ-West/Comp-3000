#pragma once

#include "gameFiles/UI/UIElement.h"
#include "SDL3/SDL.h"

class arrowButton: public UIElement {
public:
	arrowButton(SDL_FRect rSize, SDL_Texture* texture, SDL_Texture* fTexture) : UIElement(rSize), arrowTexture(texture), flippedTexture(fTexture) {
		currentTexture = arrowTexture;
	}
	~arrowButton() {}

	bool update(SDL_Keycode key) { return true; }
	void update(){
		toSecond = !toSecond;
		if (toSecond) { currentTexture = arrowTexture; }
		else { currentTexture = flippedTexture; }
	}

	void render(SDL_Renderer* renderer) {
		SDL_RenderTexture(renderer, currentTexture, NULL, &size);
	}

	bool getToSecond() { return toSecond; }

private:
	bool toSecond = true;
	SDL_FRect Normal;
	SDL_FRect flipped;

	SDL_Texture* currentTexture;

	SDL_Texture* arrowTexture;
	SDL_Texture* flippedTexture;
};