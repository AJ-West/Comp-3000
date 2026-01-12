#include "levelUI.h"
#include "transferBox.h"

levelUI::levelUI(SDL_Renderer* renderer, const char* filePath, LevelManager* lManager, dayCycle* gameTime) : UIHandler(renderer, filePath), manager(lManager), time(gameTime) {
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
	SDL_FRect timeSize{ 62.0f / 1440.0f * camera.dimen.w, 9.0f / 960.0f * camera.dimen.h , 66.0f / 1440.0f * camera.dimen.w , 30.0f / 960.0f * camera.dimen.h };
	SDL_RenderTexture(renderer, texture, NULL, &timeSize);
}

void levelUI::render() {
	if (box) {
		box->render(renderer);
	}
	SDL_FRect size{ 0, 0, camera.dimen.w, camera.dimen.h };
	SDL_RenderTexture(renderer, texture, NULL, &size);
}

void levelUI::textInput(SDL_Keycode key) {
	box->update(key);
}

void levelUI::createTransferBox(GameObject* sUnit, GameObject* sConvoy) {
	box = new transferBox({ 390.0f / 1440.0f * camera.dimen.w, 360.0f / 960.0f * camera.dimen.h, 660.0f / 1440.0f * camera.dimen.w , 240.0f / 960.0f * camera.dimen.h }, sUnit, sConvoy);
}

void levelUI::createNewUnitBox(DepotObj* gameDepot) {
	box = new unitMaker({ 390.0f / 1440.0f * camera.dimen.w, 360.0f / 960.0f * camera.dimen.h, 660.0f / 1440.0f * camera.dimen.w , 240.0f / 960.0f * camera.dimen.h }, manager, gameDepot);
}

bool levelUI::checkClickInput() { // checks if clicked on an input box in the UI
	//get click positon
	float cx, cy;
	getScaledMousePos(&cx, &cy);
	if (box) {
		if (box->checkClick(cx, cy)) {
			bool clicked = box->findClickedElement(cx, cy);
			if (clicked && box->getToDelete()) {
				delete box;
				box = nullptr;
				return false;
			}
			return true;
		}
	}
	return false;
}