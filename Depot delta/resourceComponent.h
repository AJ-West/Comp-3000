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
		clampResources(DOS);
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

	//getters
	vector<int> getResourcesCount() { return resourcesCount; }

	//setters
	void setResourceUsage(int index, int amount) { resourceUsage[index] = amount; }
	void setResourceIncrease(int index, int amount) { resourceIncrease[index] = amount; }

	resourceComponent(GameObject* obj, vector<int> max, vector<int> count): Component(obj), resourcesMax(max), resourcesCount(count) {
		resourceUsage = vector<int>(max.size(), 0);
		resourceIncrease = vector<int>(max.size(), 0);
	}
	virtual ~resourceComponent() {}
private:
	vector<int> resourcesMax;	
	vector<int> resourcesCount;	
	vector<int> resourceUsage;
	vector<int> resourceIncrease;

	int timeCount = 0;
};