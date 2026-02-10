#include "gameFiles/entities/Buildings/building.h"

BuildingObj::BuildingObj(int x, int y, int width, int height, int health, int type, bool used) : GameObject(x, y, width, height, health), rType(type) {
	setAlive(used);

    vector<int> max = { 100, 100, 100, 100, 100 };
    vector<int> count = { 5, 50, 50, 50, 50 };
    max[rType] *= 3;
    count[rType] *= 3;

    AddComponent(make_shared<resourceComponent>(this, max, count, loadResourceTextures()));
    getComponent<resourceComponent>()->setResourceChange(rType, -5);
	AddComponent(make_shared<buttonComponent>(this));
}

//update all components
void BuildingObj::Update() {
	cout << alive << '\n';
	if (alive) {
		for (auto& pair : getComponents()) {
			pair.second->update(this);
		}
	}
}

void BuildingObj::onClick() {
	selected = !selected;
}

void BuildingObj::clickAway() {
	getMapScaledMousePos(&tx, &ty);
}

void BuildingObj::renderHover(SDL_Renderer* renderer) { // needs removing when creating a subobject for zombie/convoy/unit
	auto rComp = getComponent<resourceComponent>();
	if (rComp) {
		SDL_FRect tSize{ 1254.0f * camera.xScale, 60.0f * camera.yScale, 182.0f * camera.xScale, 48.0f * camera.yScale };
		rComp->renderResources(renderer, tSize);
	}
}

void BuildingObj::render(SDL_Renderer* renderer) {
	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
	if (alive) { SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255); }
	SDL_FRect size = getDimensions();
	size.x -= camera.dimen.x;
	size.y -= camera.dimen.y;
	SDL_RenderFillRect(renderer, &size);
}