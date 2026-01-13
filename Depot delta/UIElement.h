#pragma once
#include <iostream>

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>

#include "variables.h"

class UIElement {
public:
	UIElement(SDL_FRect rSize): size(rSize) {}
	~UIElement() {}

	virtual bool update(SDL_Keycode key) = 0;

	bool checkClick(float x, float y) {
		if (x < size.x) return false;
		if (x > size.x + size.w) return false;
		if (y < size.y) return false;
		if (y > size.y + size.h) return false;
		return true;
	}

	virtual void render(SDL_Renderer* renderer) = 0;

	//getters
	virtual string getText() { return "N/A"; }
	virtual SDL_FRect getSize() { return size; }

protected:
	SDL_FRect size;
};