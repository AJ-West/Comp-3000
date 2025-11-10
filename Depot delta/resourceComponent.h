#pragma once
#include "GameObject.h"
#include <SDL3_image/SDL_image.h>

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
		clampResources(SCRAP);
	}

	void clampResources(int index) {
		int change = resourceIncrease[index] - resourceUsage[index];
		if (resourcesCount[index] + change <= resourcesMax[index] && resourcesCount[index] + change >= 0) {
			resourcesCount[index] += resourceIncrease[index] - resourceUsage[index];
		}
		else if (resourcesCount[index] + change > resourcesMax[index]) {
			resourcesCount[index] = resourcesMax[index];
		}
		else {
			resourcesCount[index] = 0;
		}
	}

	void renderResources(SDL_Renderer* renderer) {
		SDL_FRect size{camera.x + camera.width/9*2, camera.y + camera.height - 75, camera.width/18, 50};
		//SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
		//SDL_RenderRect(renderer, &size);
		for (int i = 0; i < resourceTextures.size(); i++) {
			SDL_RenderTexture(renderer, resourceTextures[i], NULL, &size);
			size.x += size.w*2;
		}
		cout << "render" << '\n';
	}

	//getters
	vector<int> getResourcesCount() { return resourcesCount; }

	//setters
	void setResourceUsage(int index, int amount) { resourceUsage[index] = amount; }
	void setResourceIncrease(int index, int amount) { resourceIncrease[index] = amount; }

	resourceComponent(GameObject* obj, vector<int> max, vector<int> count, vector<SDL_Texture*> textures): Component(obj), resourcesMax(max), resourcesCount(count), resourceTextures(textures) {
		resourceUsage = vector<int>(max.size(), 0);
		resourceIncrease = vector<int>(max.size(), 0);
	}
	virtual ~resourceComponent() {}
private:
	vector<int> resourcesMax;	
	vector<int> resourcesCount;	
	vector<int> resourceUsage;
	vector<int> resourceIncrease;
	vector<SDL_Texture*> resourceTextures;

	int timeCount = 0;
};