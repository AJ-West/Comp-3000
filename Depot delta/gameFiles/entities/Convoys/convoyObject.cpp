#include "gameFiles/entities/Convoys/convoyObject.h"
#include "gameFiles/useThroughout/stats/convoyStats.h"

ConvoyObj::ConvoyObj(int x, int y, int width, int height, int health, int id) : HumanObj(x, y, width, height, health, id) {}

void ConvoyObj::renderHover(SDL_Renderer* renderer) {
	auto rComp = getComponent<resourceComponent>();
	if (rComp) {
		SDL_FRect tSize{ 1254.0f * camera.xScale, 60.0f * camera.yScale, 182.0f * camera.xScale, 48.0f * camera.yScale };
		rComp->renderResources(renderer, tSize);
	}
	auto rTComp = getComponent<resourceTransferComponent>();
	if (rTComp) {
		rTComp->renderTransferArea();
	}
}