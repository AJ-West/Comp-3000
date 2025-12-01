#include "levelUI.h"

levelUI::levelUI(SDL_Renderer* renderer, const char* filePath, dayCycle* gameTime) : UIHandler(renderer, filePath), time(gameTime) {
	loadResourceHoverTexture();
}
levelUI::~levelUI() {}

void levelUI::loadResourceHoverTexture() {
	// Load resource hover texture
	SDL_Surface* surface = IMG_Load("art/UI/level/UnitStat.png");
	if (!surface) {
		cerr << "Unable to load image! IMG_Error: " << SDL_GetError() << endl;
		return;
	}
	resourceHoverTexture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_SetTextureScaleMode(resourceHoverTexture, SDL_SCALEMODE_PIXELART);
	SDL_DestroySurface(surface); // Free the surface after creating the texture
	if (!resourceHoverTexture) {
		cerr << "Unable to create texture! SDL_Error: " << SDL_GetError() << endl;
		return;
	}
}

void levelUI::renderResourceHover() {
	SDL_FRect size{ 1175.0f / 1440.0f * camera.dimen.w, 48.0f/960.0f * camera.dimen.h, 265.0f / 1440.0f * camera.dimen.w, 432.0f / 960.0f * camera.dimen.h }; //using dimensions from figma to scale for all zoom
	SDL_RenderTexture(renderer, resourceHoverTexture, NULL, &size);
}

void levelUI::renderTime() {
	vector<int> cTime = time->getDayTime();
	int iMin = cTime[minute];
	string sMin = to_string(iMin);
	if (iMin < 10) {
		sMin = "0" + sMin;
	}
	int iHour = cTime[hour];
	string sHour = to_string(iHour);
	if (iHour < 10) {
		sHour = "0" + sHour;
	}
	string countText = sHour + ":" + sMin;
	SDL_Surface* surface = TTF_RenderText_Solid(font, countText.c_str(), countText.length(), { 255,255,255,255 });
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_RenderTexture(renderer, texture, NULL, &timeSize);
}