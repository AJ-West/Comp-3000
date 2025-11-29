#pragma once
#include <iostream>
#include <vector>

#include <SDL3/SDL.h>

#include "variables.h"

//note to self need to ebuild for changes from this file for some reason

using namespace std;

class UIHandler {
public:
	UIHandler(SDL_Renderer* SDL_Renderer, const char* filePath): renderer(SDL_Renderer) {
		SDL_Surface* surface = IMG_Load(filePath);
		if (!surface) {
			cerr << "Unable to load image! IMG_Error: " << SDL_GetError() << endl;
			return;
		}
		texture = SDL_CreateTextureFromSurface(renderer, surface);
		SDL_SetTextureScaleMode(texture, SDL_SCALEMODE_PIXELART);
		SDL_DestroySurface(surface); // Free the surface after creating the texture
		if (!texture) {
			cerr << "Unable to create texture! SDL_Error: " << SDL_GetError() << endl;
			return;
		}
	}
	~UIHandler() {}

	void render() {
		//SDL_FRect size{ 0, 0, ResolutionWidth*(zoom/2), ResolutionHeight*(zoom/2)};
		SDL_FRect size{ 0, 0, camera.dimen.w, camera.dimen.h};
		//cout << "camera x" << camera.dimen.x << '\n';
		//cout << "camera y" << camera.dimen.y << '\n';
		//cout << "text x" << size.x << '\n';
		//cout << "text y" << size.y << '\n';
		SDL_RenderTexture(renderer, texture, NULL, &size);
	}

protected:
	SDL_Renderer* renderer;
private:	
	SDL_Texture* texture;
};