#include "gameFiles/entities/Buildings/building.h"

BuildingObj::BuildingObj(int x, int y, int width, int height, int health, bool used, int type, int ID) : GameObject(x, y, width, height, health), rType(type), id(ID) {
	alive = used;
}

//update all components
void BuildingObj::Update() {
	if (alive) {
		for (auto& pair : getComponents()) {
			pair.second->update();
		}
	}
	else {
		getComponent<renderComponent>()->update();
		getComponent<buttonComponent>()->update();
	}
}

void BuildingObj::onClick() {
	selected = !selected;
}

void BuildingObj::clickAway() {
	getMapScaledMousePos(&tx, &ty);
}

void BuildingObj::renderHover(SDL_Renderer* renderer) {
	auto rComp = getComponent<resourceComponent>();
	if (rComp) {
		SDL_FRect tSize{ 1254.0f * camera.xScale, 60.0f * camera.yScale, 182.0f * camera.xScale, 48.0f * camera.yScale };
		rComp->renderResources(renderer, tSize);
	}
}

void BuildingObj::produceResources(bool produce) {
	auto rComp = getComponent<resourceComponent>();
	int change = -1;
	if (produce) {
		change = 1;
	}
	rComp->setResourceChange(PERSONNEL, change);
	rComp->setResourceChange(AMMUNITION, change);
	rComp->setResourceChange(DOS, change);
	rComp->setResourceChange(FUEL, change);
	rComp->setResourceChange(SCRAP, change);
	rComp->setResourceChange(rType, 3);
}