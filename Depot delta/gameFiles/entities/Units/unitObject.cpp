#pragma once
#include "gameFiles/entities/Units/unitObject.h"
#include "gameFiles/useThroughout/stats/unitStats.h"

UnitObj::UnitObj(int x, int y, int width, int height, int health, int id) : HumanObj(x, y, width, height, health, id) {}

void UnitObj::renderHover(SDL_Renderer* renderer) {
	auto rComp = getComponent<resourceComponent>();
	if (rComp) {
		SDL_FRect tSize{ 1254.0f * camera.xScale, 60.0f * camera.yScale, 182.0f * camera.xScale, 48.0f * camera.yScale };
		rComp->renderResources(renderer, tSize);
	}
}

void UnitObj::updateTargets(vector<ZombieObj*> list) {
	getComponent<attackComponent>()->setPotentialTargets(list);
};

float UnitObj::getRateOfFire() {
	return unitStats.getROF();
}