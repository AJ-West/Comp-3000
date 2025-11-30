#pragma once
#include <iostream>
#include <vector>

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>

#include "variables.h"

//note to self need to ebuild for changes from this file for some reason

using namespace std;

class UIHandler {
public:
	UIHandler(SDL_Renderer* SDL_Renderer, const char* filePath);
	~UIHandler();

	void render();

protected:
	SDL_Renderer* renderer;
private:	
	SDL_Texture* texture;
};