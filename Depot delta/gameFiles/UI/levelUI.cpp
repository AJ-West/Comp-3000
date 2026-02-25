#include "gameFiles/UI/levelUI.h"

#include "gameFiles/entities/gameObject.h"
#include "gameFiles/UI/techTree/techTree.h"

levelUI::levelUI(SDL_Renderer* renderer, const char* filePath, LevelManager* lManager, dayCycle* gameTime) : UIHandler(renderer, filePath), manager(lManager), time(gameTime) {
	loadResourceHoverTexture();

	minimap = new Minimap({ 0,0,0,0 }, manager->getAllObjects(), manager->getZombieList());
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
	SDL_FRect size{ 1175.0f * camera.xScale, 48.0f * camera.yScale, 265.0f * camera.xScale, 432.0f * camera.yScale }; //using dimensions from figma to scale for all zoom
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
	string countText = "Day: " + to_string(cTime[day]) + ". " + sHour + ":" + sMin;
	SDL_Surface* surface = TTF_RenderText_Solid(font, countText.c_str(), countText.length(), { 0,0,0,255 });
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FRect timeSize{ 62.0f * camera.xScale, 9.0f * camera.yScale, 96.0f * camera.xScale, 30.0f * camera.yScale };
	SDL_RenderTexture(renderer, texture, NULL, &timeSize);

	if (time->getSpawnSwarm()) {
		//manager->spawnSwarmThread(time->getSpawnQuantity(), time->getSpawnDirection());
		manager->spawnSwarm(time->getSpawnQuantity(), time->getSpawnDirection());
		time->swarmSpawned();
	}
}

void levelUI::render() {
	if (tree) {
		tree->render(renderer);
	}
	else {
		if (box) {
			box->render(renderer);
		}
		SDL_FRect size{ 0, 0, camera.dimen.w, camera.dimen.h };
		SDL_RenderTexture(renderer, texture, NULL, &size);
		minimap->render(renderer);
	}
}

void levelUI::textInput(SDL_Keycode key) {
	box->update(key);
}

void levelUI::createTransferBox(GameObject* sUnit, GameObject* sConvoy) {
	box = new transferBox(manager, { 220.0f * camera.xScale, 20.0f * camera.yScale, 1000.0f * camera.xScale, 1000.0f * camera.yScale }, sUnit, sConvoy);
}

void levelUI::createNewUnitBox(DepotObj* gameDepot, LevelManager* lManager) {
	box = new unitMaker({ 220.0f * camera.xScale, 20.0f * camera.yScale, 1000.0f * camera.xScale, 1000.0f * camera.yScale }, lManager, gameDepot);
}

void levelUI::createTechTree() {
	tree = new TechTree(renderer, manager, manager->getDepot().get());
}

bool levelUI::checkClickInput() { // checks if clicked on an input box in the UI
	//get click positon
	float cx, cy;
	getUIScaledMousePos(&cx, &cy);
	if (tree != nullptr) {
		if (tree->checkClick(cx, cy)) {
			bool clicked = tree->findClickedElement(cx, cy);
			return clicked;
		}
	}
	if (box != nullptr) {
		if (box->checkClick(cx, cy)) {
			bool clicked = box->findClickedElement(cx, cy);
			//if (clicked && box->getToDelete()) {
				//delete box;
				//box = nullptr;
			//}
			return clicked;
		}
	}
	if (minimap->checkForToggle(cx, cy)) {
		minimap->toggle();
		return true;
	}
	return false;
}

void levelUI::deleteBox() {
	if (box) {
		box->unPause();
		delete box;
		box = nullptr;
	}
}

void levelUI::deleteTree() {
	if (tree) {
		tree->saveTechFile();
		tree->unPause();
		delete tree;
		tree = nullptr;
	}
}