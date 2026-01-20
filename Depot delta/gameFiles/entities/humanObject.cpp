#include "gameFiles/entities/humanObject.h"

#include "gameFiles/components/resourceComponent.h"
#include "gameFiles/components/pathfindingComponent.h"

void HumanObj::onClick() {
	selected = !selected;
}

void HumanObj::clickAway() { // set target pos to clicked position
	getMapScaledMousePos(&tx, &ty);
	pathToTarget();
	getComponent<resourceComponent>()->setResourceChange(FUEL, -1);
}

void HumanObj::pathToTarget() {
	Vec2 target = { tx,ty };
	SDL_FRect size = getDimensions();
	Vec2 origin = { size.x + size.w / 2, size.y + size.h / 2 };
	auto pathComp = getComponent<pathfindingComponent>();
	pathComp->computeFlowField(target, origin);
}