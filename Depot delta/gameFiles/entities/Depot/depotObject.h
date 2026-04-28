#pragma once
#include "gameFiles/entities/gameObject.h"
#include "gameFiles/components/resourceComponent.h"
#include "gameFiles/misc/resourceOut.h"

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
			tSize.w = 94.0f * camera.xScale;
			tSize.h = 30.0f * camera.yScale;

			string countText = to_string(health) + "/" + to_string(maxHealth);
			SDL_Surface* surface = TTF_RenderText_Solid(font, countText.c_str(), countText.length(), { 0,0,0,255 });
			SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
			SDL_RenderTexture(renderer, texture, NULL, &tSize);

			tSize.x = temp[0] * camera.dimen.w;

			//tSize.x = 65;
			for (int i = 0; i < resourceCount.size(); i++) {
				string countText = to_string(resourceCount[i]) + "/" + to_string(resourceMax[i]);
				SDL_Surface* surface = TTF_RenderText_Solid(font, countText.c_str(), countText.length(), { 0,0,0,255 });
				SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
				SDL_RenderTexture(renderer, texture, NULL, &tSize);
				tSize.x = temp[i+1] * camera.dimen.w;

				checkResources(i, resourceCount, resourceMax); // see if any audio should be played
			}
		}
		if (resourceOut) {
			resourceOut->update(renderer);
			if (!resourceOut->getAlive()) {
				delete resourceOut;
				resourceOut = nullptr;
			}
		}
	}

	void checkResources(int i, vector<int> resourceCount, vector<int> resourceMax) {
		checkResourceMax(i, resourceCount, resourceMax);
		if (i == SCRAP) { return; }
		checkResourceLow(i, resourceCount, resourceMax);
		checkResourceOut(i, resourceCount, resourceMax);
	}

	void checkResourceMax(int i, vector<int> resourceCount, vector<int> resourceMax) {
		if (!justReachedMax[i] && resourceCount[i] == resourceMax[i]) {
			soundEffectEngine->play2D(pickRandomFile(reachedMaxAudio));
			justReachedMax[i] = true;
		}
		else if (justReachedMax[i] && resourceCount[i] != resourceMax[i]) {
			justReachedMax[i] = false;
		}
	}

	void checkResourceLow(int i, vector<int> resourceCount, vector<int> resourceMax) {
		if (!justReachedLow[i] && resourceCount[i] <= resourceMax[i]/10) {
			soundEffectEngine->play2D(pickRandomFile(reachedLowAudio));
			justReachedLow[i] = true;
		}
		else if (justReachedLow[i] && resourceCount[i] > resourceMax[i]/10) {
			justReachedLow[i] = false;
		}
	}

	void checkResourceOut(int i, vector<int> resourceCount, vector<int> resourceMax) {
		if (!justReachedOut[i] && resourceCount[i] == 0) {
			soundEffectEngine->play2D(pickRandomFile(reachedOutAudio));
			justReachedOut[i] = true;
			resourceOut = new ResourceOut(this, i);
		}
		else if (justReachedOut[i] && resourceCount[i] != 0) {
			justReachedOut[i] = false;
		}
	}

	void updateStats(string keyName) {
		if (keyName == "depotHealthIncrease") {
			maxHealth *= depotTechVal[keyName];
		}
		else if (keyName == "fuelProductionIncrease") {
			getComponent<resourceComponent>()->adjustResourceChange(FUEL, 1* depotTechVal[keyName]);
		}
		else if (keyName == "dosProductionIncrease") {
			getComponent<resourceComponent>()->adjustResourceChange(DOS, 1* depotTechVal[keyName]);
		}
		else if (keyName == "ammoProductionIncrease") {
			getComponent<resourceComponent>()->adjustResourceChange(AMMUNITION, 1* depotTechVal[keyName]);
		}
		else if (keyName == "increasePersonnelCapacity") {
			getComponent<resourceComponent>()->setResourceMax(PERSONNEL, getComponent<resourceComponent>()->getResourcesMax(PERSONNEL)*2);
		}
	}

	void produceResources(bool produce) {
		auto rComp = getComponent<resourceComponent>();
		rComp->setResourceChange(PERSONNEL, 1);
		rComp->setResourceChange(AMMUNITION, 1 * depotTechVal["ammoProductionIncrease"]);
		rComp->setResourceChange(DOS, 1 * depotTechVal["dosProductionIncrease"]);
		rComp->setResourceChange(FUEL, 1 * depotTechVal["fuelProductionIncrease"]);
		rComp->setResourceChange(SCRAP, 1);
	}

	virtual void underAttack() {
		soundEffectEngine->play2D(pickRandomFile(underAttackAudio));
	}

	virtual int getType() { return DEPOT; }

private:
	SDL_FRect iSize{ camera.dimen.x + 10, camera.dimen.y + 5, 30, 30 };
	//SDL_FRect tSize{ 312/ static_cast<float>(ResolutionWidth) *100, 5 / static_cast<float>(ResolutionWidth) * 100, 111 / camera.dimen.w * 100, 30 / camera.dimen.h * 100 };
	SDL_FRect tSize{ 312.0f * camera.xScale, 9.0f * camera.yScale, 111.0f * camera.xScale, 30.0f * camera.yScale };
	vector<float> temp{ 464.0f/1440.0f, 0.425, 0.5291, 0.6347, 0.7402, 0 };

	vector<const char*> underAttackAudio{ "soundEffects/voice acting/depot/underAttack/ditd.wav", "soundEffects/voice acting/depot/underAttack/dua.wav", "soundEffects/voice acting/depot/underAttack/zhrtd.wav" };

	vector<bool> justReachedMax{ false, false, false, false, false };
	vector<const char*> reachedMaxAudio{ "soundEffects/voice acting/depot/maxingResource/personnel.wav", "soundEffects/voice acting/depot/maxingResource/ammo.wav", "soundEffects/voice acting/depot/maxingResource/dos.wav", "soundEffects/voice acting/depot/maxingResource/fuel.wav", "soundEffects/voice acting/depot/maxingResource/scrap.wav" };

	vector<bool> justReachedLow{ false, false, false, false };
	vector<const char*> reachedLowAudio{ "soundEffects/voice acting/depot/resourcesLow/personnel.wav", "soundEffects/voice acting/depot/resourcesLow/ammo.wav", "soundEffects/voice acting/depot/resourcesLow/dos.wav", "soundEffects/voice acting/depot/resourcesLow/fuel.wav" };

	vector<bool> justReachedOut{ false, false, false, false };
	vector<const char*> reachedOutAudio{ "soundEffects/voice acting/depot/resourcesOut/personnel.wav", "soundEffects/voice acting/depot/resourcesOut/ammo.wav", "soundEffects/voice acting/depot/resourcesOut/dos.wav", "soundEffects/voice acting/depot/resourcesOut/fuel.wav" };

	ResourceOut* resourceOut = nullptr;

	//SDL_FRect tSize{ 65, 5, 23, 30 / camera.dimen.h * 100 };
};