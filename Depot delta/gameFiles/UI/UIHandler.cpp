#include "UIHandler.h"
#include "gameFiles/useThroughout/usefulFunctions.h"

UIHandler::UIHandler(SDL_Renderer* SDL_Renderer, const char* filePath) : renderer(SDL_Renderer) {
	texture = loadTexture(filePath);
}

UIHandler::~UIHandler(){}

void UIHandler::render() {
	SDL_FRect size{ 0, 0, camera.dimen.w, camera.dimen.h };
	SDL_RenderTexture(renderer, texture, NULL, &size);
}