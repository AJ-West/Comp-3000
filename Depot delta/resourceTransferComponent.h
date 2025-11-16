#pragma once
#include "GameObject.h"
#include <SDL3_image/SDL_image.h>

class resourceTransferComponent : public Component {// renderers the object
public:
	virtual void update(GameObject* owner) { // render the current frame
		if (transfering) { 
			renderTransferIndicator();
			checkTransfer(); 
		}
	}

	bool checkDistance() {
		//TO-DO: check distance between depot and convoy to transfer resources
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

	void initiateTransfer(GameObject* targetobj) {
		target = targetobj;
		//TO-DO: transfer resources between depot and convoy
		if (checkDistance()) {
			transfering = true;
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
		cout << "Stopping resource transfer" << endl;
	}

	void renderTransferArea() {
		SDL_SetRenderDrawColor(renderer, 0, 255, 0, 100); // Green color
		SDL_FRect ownerRect = owner->getSize();
		SDL_FRect indicatorRect = { ownerRect.x+ownerRect.w/2 - transferDistance /2, ownerRect.y + ownerRect.h/2 - transferDistance / 2, transferDistance, transferDistance };
		SDL_RenderFillRect(renderer, &indicatorRect);
	}

	void renderTransferIndicator() {
		SDL_SetRenderDrawColor(renderer, 0, 255, 255, 255);
		SDL_FRect ownerRect = owner->getSize();
		SDL_FRect targetRect = target->getSize();
		SDL_RenderLine(renderer, ownerRect.x + ownerRect.w / 2, ownerRect.y + ownerRect.h / 2, targetRect.x + targetRect.w / 2, targetRect.y + targetRect.h / 2);
	}

	resourceTransferComponent(GameObject* obj, SDL_Renderer* SDL_Renderer, float distance) : Component(obj), renderer(SDL_Renderer), transferDistance(distance) {	}
	virtual ~resourceTransferComponent() {}
private:
	GameObject* target;
	SDL_Renderer* renderer;

	bool transfering = false;

	float transferDistance;
};