#pragma once
#include "gameFiles/entities/gameObject.h"
#include "gameFiles/components/resourceComponent.h"
#include <SDL3_image/SDL_image.h>

class resourceTransferComponent : public Component {// renderers the object
public:
	virtual void update(GameObject* owner) { // render the current frame
		if (transfering) { 
			renderTransferIndicator();
			checkTime();
			checkTransfer(); 
		}
	}

	void checkTime() {
		timeCount += 1;
		if (timeCount == 60) {
			timeCount = 0;
			transferResource();
		}
	}

	void transferResource() {
		bool increase = false;
		bool decrease = false;
		bool change = false;
		//amount of a resource transfered
		int transferNum = 0;
		for (int i = 0; i < transferRate.size(); i++) {
			transferNum = 0;
			if (transferAmount[i] == 0) {
				ownerResComp->setResourceChange(i, 0);
				targetResComp->setResourceChange(i, 0);
				continue;
			} 
			else if (transferAmount[i] > 0) {
				if (ownerResComp->getResourcesCount(i) < transferRate[i]) { // transfer last of the resource amount
					transferNum = ownerResComp->getResourcesCount(i);
					transferRate[i] = 0;
					increase = true;
				}
				else if (targetResComp->getResourcesMax(i) < targetResComp->getResourcesCount(i) + transferRate[i]) { // transfer enough to fill up the target
					transferNum = targetResComp->getResourcesMax(i) - targetResComp->getResourcesCount(i);
					increase = true;
				}
				else if (targetResComp->getResourcesMax(i) != targetResComp->getResourcesCount(i)) { // transfer normally
					transferNum = transferRate[i];
					increase = true;
				}
				if (increase) {
					if (transferAmount[i] < transferNum) { // if transferNum would transfer more than requested
						transferNum = transferAmount[i];
					}
					ownerResComp->setResourceChange(i, -transferNum);
					targetResComp->setResourceChange(i, transferNum);
					transferAmount[i] = transferAmount[i] - transferNum;
					change = true;
				}
			}
			else if (transferAmount[i] < 0) {
				if (targetResComp->getResourcesCount(i) < transferRate[i]) { // transfer last of the resource amount
					transferNum = targetResComp->getResourcesCount(i);
					transferRate[i] = 0;
					decrease = true;
				}
				else if (ownerResComp->getResourcesMax(i) < ownerResComp->getResourcesCount(i) + transferRate[i]) { // transfer enough to fill up the target
					transferNum = ownerResComp->getResourcesMax(i) - ownerResComp->getResourcesCount(i);
					decrease = true;
				}
				else if (ownerResComp->getResourcesMax(i) != ownerResComp->getResourcesCount(i)) { // transfer normally
					transferNum = transferRate[i];
					decrease = true;
				}
				if (decrease) {
					if (mag(transferAmount[i]) < transferNum) { // if transferNum would transfer more than requested
						transferNum = transferAmount[i];
					}
					ownerResComp->setResourceChange(i, transferNum);
					targetResComp->setResourceChange(i, -transferNum);
					transferAmount[i] = transferAmount[i] + transferNum;
					change = true;
				}
			}
		}
		if (!change) {
			stopTransfer();
		}
	}

	bool checkDistance() {
		SDL_FRect ownerRect = owner->getSize();
		SDL_FRect targetRect = target->getSize();
		float dx = (ownerRect.x + ownerRect.w / 2) - (targetRect.x + targetRect.w / 2);
		float dy = (ownerRect.y + ownerRect.h / 2) - (targetRect.y + targetRect.h / 2);
		float distance = sqrt(dx * dx + dy * dy);
		if (distance <= transferDistance) {
			return true;
		}
		return false;
	}

	bool checkDistance(SDL_FRect ownerRect, SDL_FRect targetRect) {
		float dx = (ownerRect.x + ownerRect.w / 2) - (targetRect.x + targetRect.w / 2);
		float dy = (ownerRect.y + ownerRect.h / 2) - (targetRect.y + targetRect.h / 2);
		float distance = sqrt(dx * dx + dy * dy);
		//if (distance <= transferDistance) {
		if (distance <= 1000) {
			return true;
		}
		return false;
	}

	void initiateTransfer(GameObject* targetobj, vector<int> toTransfer) {
		target = targetobj;
		transferAmount = toTransfer;
		//TO-DO: transfer resources between depot and convoy
		if (checkDistance()) {
			transfering = true;
			ownerResComp = owner->getComponent<resourceComponent>();
			targetResComp = target->getComponent<resourceComponent>();
			cout << "Initiating resource transfer" << endl;
		}
		else {
			target = nullptr;
		}
	}

	void checkTransfer() {
		if (transfering && !checkDistance()) {
			stopTransfer();
		}
	}

	void stopTransfer() {
		transfering = false;
		target = nullptr;
		ownerResComp = nullptr;
		targetResComp = nullptr;
		cout << "Stopping resource transfer" << endl;
	}

	void renderTransferArea() {
		SDL_SetRenderDrawColor(renderer, 0, 255, 0, 100); // Green color
		SDL_FRect ownerRect = owner->getSize();
		SDL_FRect indicatorRect = { ownerRect.x + ownerRect.w / 2 - transferDistance / 2 - camera.dimen.x, ownerRect.y + ownerRect.h / 2 - transferDistance / 2 - camera.dimen.y, transferDistance, transferDistance };
		SDL_RenderFillRect(renderer, &indicatorRect);
	}

	void renderTransferIndicator() {
		SDL_SetRenderDrawColor(renderer, 0, 255, 255, 255);
		SDL_FRect ownerRect = owner->getSize();
		SDL_FRect targetRect = target->getSize();
		SDL_RenderLine(renderer, ownerRect.x + ownerRect.w / 2 - camera.dimen.x, ownerRect.y + ownerRect.h / 2 - camera.dimen.x, targetRect.x + targetRect.w / 2, targetRect.y + targetRect.h / 2);
	}

	//getters
	GameObject* getTarget() { return target; }
	bool getTransfering() { return transfering; }
	int getTransferAmount(int index) { return transferAmount[index]; }

	resourceTransferComponent(GameObject* obj, SDL_Renderer* SDL_Renderer, float distance, vector<int> resourceTransferRate) : Component(obj), renderer(SDL_Renderer), transferDistance(distance), transferRate(resourceTransferRate) {	}
	virtual ~resourceTransferComponent() {}
private:
	GameObject* target = nullptr;
	SDL_Renderer* renderer;

	shared_ptr<resourceComponent> ownerResComp = nullptr;
	shared_ptr<resourceComponent> targetResComp = nullptr;

	vector<int> transferRate;

	vector<int> transferAmount{ 0,0,0,0,0 };

	bool transfering = false;

	float transferDistance;

	int timeCount = 0;
};