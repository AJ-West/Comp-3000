#pragma once
#include "gameFiles/entities/humanObject.h"

#include "gameFiles/components/renderComponent.h"
#include "gameFiles/components/buttonComponent.h"
#include "gameFiles/components/movementComponent.h"
#include "gameFiles/components/resourceComponent.h"
#include "gameFiles/components/resourceTransferComponent.h"
#include "gameFiles/components/pathfindingComponent.h"

class ConvoyObj : public HumanObj {
public:
	ConvoyObj(int x, int y, int width, int height, int health, int id) : HumanObj(x, y, width, height, health, id) {}
	
	virtual void renderHover(SDL_Renderer* renderer) {
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
};

struct convoyStats {
	const char* art = "draftArt/basicConvoy.png";
	int movementSpeed = 100;
	int maxHealth = 100;
	std::vector<int> rMax = { 100, 100, 100, 100, 100 };
	std::vector<int> rCount = { 50, 50, 50, 50, 50 };

	int rTransferDistance = 50;
	std::vector<int> rTransferRate = { 5,5,5,5,5 };

	void addComponents(ConvoyObj* convoy) {
		convoy->AddComponent(make_shared<renderComponent>(convoy, renderer, art));
		convoy->AddComponent(make_shared<buttonComponent>(convoy));
		convoy->AddComponent(make_shared<movementComponent>(convoy, movementSpeed));
		convoy->AddComponent(make_shared<resourceComponent>(convoy, rMax, rCount, loadResourceTextures()));
		convoy->AddComponent(make_shared<resourceTransferComponent>(convoy, renderer, rTransferDistance, rTransferRate));
		convoy->AddComponent(make_shared<pathfindingComponent>(convoy, grid));
	}
};