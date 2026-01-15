#pragma once
#include "gameFiles/entities/gameObject.h"
#include "gameFiles/components/resourceComponent.h"

class DepotObj : public GameObject{
public:
	DepotObj(int x, int y, int width, int height, int health) : GameObject(x, y, width, height, health) {}

	void onClick() {
		selected = !selected;
	}

	void clickAway() {
		getMapScaledMousePos(&tx, &ty);
	}

	void renderHover(SDL_Renderer* renderer) { // needs removing when creating a subobject for zombie/convoy/unit
		cout << "renderHover";
	}

	void renderResources(SDL_Renderer* renderer) {
		auto rComp = getComponent<resourceComponent>();
		if (rComp) {
			vector<int> resourceCount = rComp->getAllResourceCount();
			vector<int> resourceMax = rComp->getAllResourceMax();
			//start x pos 
			//tSize.x = 312 / static_cast<float>(ResolutionWidth) * 100;
			tSize.x = 312.0f * camera.xScale;
			tSize.y = 9.0f * camera.yScale;
			tSize.w = 111.0f * camera.xScale;
			tSize.h = 30.0f * camera.yScale;

			string countText = to_string(health) + "/" + to_string(maxHealth);
			SDL_Surface* surface = TTF_RenderText_Solid(font, countText.c_str(), countText.length(), { 255,255,255,255 });
			SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
			SDL_RenderTexture(renderer, texture, NULL, &tSize);

			tSize.x = temp[0] * camera.dimen.w;

			//tSize.x = 65;
			for (int i = 0; i < resourceCount.size(); i++) {
				string countText = to_string(resourceCount[i]) + "/" + to_string(resourceMax[i]);
				SDL_Surface* surface = TTF_RenderText_Solid(font, countText.c_str(), countText.length(), { 255,255,255,255 });
				SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
				SDL_RenderTexture(renderer, texture, NULL, &tSize);
				//tSize.x += 152 / static_cast<float>(ResolutionWidth) * 100;
				tSize.x = temp[i+1] * camera.dimen.w;
				//tSize.x += 31.5;
			}
		}
	}

private:
	SDL_FRect iSize{ camera.dimen.x + 10, camera.dimen.y + 5, 30, 30 };
	//SDL_FRect tSize{ 312/ static_cast<float>(ResolutionWidth) *100, 5 / static_cast<float>(ResolutionWidth) * 100, 111 / camera.dimen.w * 100, 30 / camera.dimen.h * 100 };
	SDL_FRect tSize{ 312.0f * camera.xScale, 9.0f * camera.yScale, 111.0f * camera.xScale, 30.0f * camera.yScale };
	vector<float> temp{ 464.0f/1440.0f, 0.4215, 0.5291, 0.6347, 0.7402, 0 };
	//SDL_FRect tSize{ 65, 5, 23, 30 / camera.dimen.h * 100 };
};