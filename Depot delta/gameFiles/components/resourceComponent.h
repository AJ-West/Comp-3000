#pragma once
#include "gameFiles/entities/gameObject.h"
#include <SDL3_image/SDL_image.h>
#include "gameFiles/components/movementComponent.h"

class resourceComponent : public Component {// renderers the object
public:
	virtual void update(GameObject* owner) { // render the current frame
		checkTime();
	}

	void checkTime() {
		timeCount += 1;
		if (timeCount == 60) {
			timeCount = 0;
			updateResource();
		}
	}

	void updateResource() {
		clampResources(PERSONNEL);
		clampResources(AMMUNITION);
		clampResources(DOS);
		clampResources(FUEL);
		if (hasResource[FUEL] != tempHasResource[FUEL]) { checkFuel(); }
		clampResources(SCRAP);
	}

	void clampResources(int index) {
		tempHasResource[index] = true;
		if (resourcesCount[index] + resourceChange[index] <= resourcesMax[index] && resourcesCount[index] + resourceChange[index] >= 0) {
			resourcesCount[index] += resourceChange[index];
		}
		else if (resourcesCount[index] + resourceChange[index] > resourcesMax[index]) {
			resourcesCount[index] = resourcesMax[index];
		}
		else {
			tempHasResource[index] = false;
			resourcesCount[index] = 0;
		}
	}

	void checkFuel() {
		hasResource[FUEL] = tempHasResource[FUEL];
		auto mComp = owner->getComponent<movementComponent>();
		if (mComp) {
			if (hasResource[FUEL]) {
				mComp->setSpeed(mComp->getSpeed() * 2);
			}
			else
			{
				mComp->setSpeed(mComp->getSpeed() / 2);
			}
		}
	}

	void renderResources(SDL_Renderer* renderer, SDL_FRect tSize) {
		string countText = to_string(owner->getHealth()) + "/" + to_string(owner->getMaxHealth());
		SDL_Surface* surface = TTF_RenderText_Solid(font, countText.c_str(), countText.length(), { 0,0,0,255 });
		SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
		SDL_RenderTexture(renderer, texture, NULL, &tSize);
		tSize.y += 72.0f * camera.yScale;
		for (int i = 0; i < resourceTextures.size(); i++) {
			string countText = to_string(resourcesCount[i]) + "/" + to_string(resourcesMax[i]);
			SDL_Surface* surface = TTF_RenderText_Solid(font, countText.c_str(), countText.length(), { 0,0,0,255 });
			SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
			SDL_RenderTexture(renderer, texture, NULL, &tSize);
			tSize.y += 72.0f/960.0f * camera.dimen.h;
		}
	}

	//getters
	vector<int> getAllResourceCount() { return resourcesCount; }
	vector<int> getAllResourceMax() { return resourcesMax; }
	int getResourcesCount(int index) { return resourcesCount[index]; }
	int getResourcesMax(int index) { return resourcesMax[index]; }

	//setters
	void setResourceChange(int index, int amount) { resourceChange[index] = amount; }
	void setResourcesCount(int index, int amount) { resourcesCount[index] = amount; }
	void setHasResource(vector<bool> loadHasResource) { hasResource = loadHasResource; }

	resourceComponent(GameObject* obj, vector<int> max, vector<int> count, vector<SDL_Texture*> textures): Component(obj), resourcesMax(max), resourcesCount(count), resourceTextures(textures) {
		resourceChange = vector<int>(max.size(), 0);
	}
	virtual ~resourceComponent() {}
private:
	vector<int> resourcesMax;	
	vector<int> resourcesCount;	
	vector<int> resourceChange;
	vector<SDL_Texture*> resourceTextures;
	vector<bool> hasResource = { true, true, true, true, true };
	vector<bool> tempHasResource = { true, true, true, true, true };

	int timeCount = 0;
};