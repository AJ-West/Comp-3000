#pragma once
#include "GameObject.h"
#include "buttonBase.h"
#include "resourceComponent.h"

class DepotObj : public GameObject, public BaseButton {
public:
	DepotObj(int x, int y, int width, int height) : GameObject(x, y, width, height) {}

	void onClick() {
		selected = !selected;
	}

	void clickAway() {
		getScaledMousePos(&tx, &ty);
	}

	void renderHover(SDL_Renderer* renderer) {
		cout << "renderHover";
	}

	void renderResources(SDL_Renderer* renderer) {
		auto rComp = getComponent<resourceComponent>();
		if (rComp) {
			vector<int> resourceCount = rComp->getAllResourceCount();
			vector<int> resourceMax = rComp->getAllResourceMax();
			//start x pos 
			//tSize.x = 312 / static_cast<float>(ResolutionWidth) * 100;
			tSize.x = 0.322f * camera.dimen.w;
			tSize.y = 0.005f * camera.dimen.h;
			tSize.w = 0.077f * camera.dimen.w;
			tSize.h = 0.03125f * camera.dimen.h;
			//tSize.x = 65;
			for (int i = 0; i < resourceCount.size(); i++) {
				string countText = to_string(resourceCount[i]) + "/" + to_string(resourceMax[i]);
				SDL_Surface* surface = TTF_RenderText_Solid(font, countText.c_str(), countText.length(), { 255,255,255,255 });
				SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
				SDL_RenderTexture(renderer, texture, NULL, &tSize);
				//tSize.x += 152 / static_cast<float>(ResolutionWidth) * 100;
				tSize.x = temp[i] * camera.dimen.w;
				//tSize.x += 31.5;
			}
		}
	}

private:
	SDL_FRect iSize{ camera.dimen.x + 10, camera.dimen.y + 5, 30, 30 };
	//SDL_FRect tSize{ 312/ static_cast<float>(ResolutionWidth) *100, 5 / static_cast<float>(ResolutionWidth) * 100, 111 / camera.dimen.w * 100, 30 / camera.dimen.h * 100 };
	SDL_FRect tSize{ 0.1625f * camera.dimen.w, 0.005f * camera.dimen.h, 0.0575f * camera.dimen.w, 0.02f * camera.dimen.h };
	vector<float> temp{ 0.4215, 0.5291, 0.6347, 0.7402, 0 };
	//SDL_FRect tSize{ 65, 5, 23, 30 / camera.dimen.h * 100 };
};