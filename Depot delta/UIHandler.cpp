#include "UIHandler.h"

UIHandler::UIHandler(SDL_Renderer* SDL_Renderer, const char* filePath) : renderer(SDL_Renderer) {
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

UIHandler::~UIHandler(){}

void UIHandler::render() {
	SDL_FRect size{ 0, 0, camera.dimen.w, camera.dimen.h };
	SDL_RenderTexture(renderer, texture, NULL, &size);
}