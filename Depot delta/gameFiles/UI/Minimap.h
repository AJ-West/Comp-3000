#pragma once
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>

#include <thread>

#include "gameFiles/UI/UIElement.h"

class Minimap : public UIElement {
public:
	Minimap(SDL_FRect size, shared_ptr<vector<shared_ptr<GameObject>>> objects, shared_ptr<vector<shared_ptr<ZombieObj>>> zombies) : UIElement(size), allObjects(objects), allZombies(zombies){
		innerSize.x = size.x + size.w / 10;
		innerSize.y = size.y + size.h / 10;
		innerSize.w = 8 * size.w / 10;
		innerSize.h = 8 * size.h / 10;
	}
	~Minimap(){}

	void updateSize() {
		size.x = 0.0f;
		size.y = camera.dimen.h - sideLength * camera.yScale;
		size.w = sideLength * camera.xScale;
		size.h = sideLength * camera.yScale;
		if (open) {
			updateInnerSize();
			updateCloseButton();
		}
	}

	void updateInnerSize() {
		innerSize.x = size.x + size.w / 15;
		innerSize.y = size.y + size.h / 15;
		innerSize.w = scaleFactor * size.w;
		innerSize.h = scaleFactor * size.h;
	}

	void updateCloseButton() {
		closeButton.x = size.x + size.w - size.w * (1-scaleFactor)/2;
		closeButton.y = size.y;
		closeButton.w = size.w * (1 - scaleFactor)/2;
		closeButton.h = size.h * (1 - scaleFactor)/2;
	}

	void render(SDL_Renderer* renderer) {
		updateSize();
		if (open) {
			SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
			SDL_RenderFillRect(renderer, &size);
			SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
			SDL_RenderFillRect(renderer, &innerSize);
			SDL_SetRenderDrawColor(renderer, 255, 165, 0, 255);
			SDL_RenderFillRect(renderer, &closeButton);
			calculateMapPositions();
		}
	}

	void calculateMapPositions() {
		thread objs(&Minimap::objectPositions, this);
		thread zombs(&Minimap::zombiePositions, this);

		objs.join();
		zombs.join();
	}

	void objectPositions() {		
		for (auto obj : *allObjects) {
			if (typeid(*obj).name() == typeid(UnitObj).name()) {
				SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
			}
			else if (typeid(*obj).name() == typeid(ConvoyObj).name()) {
				SDL_SetRenderDrawColor(renderer, 255, 150, 0, 255);
			}
			else if (typeid(*obj).name() == typeid(DepotObj).name()) {
				SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
			}


			SDL_FRect dimen = obj->getDimensions();
			float xProportion = dimen.x / worldWidth;
			float yProportion = dimen.y / worldHeight;

			SDL_FRect icon{ innerSize.x + xProportion * innerSize.w, innerSize.y + yProportion * innerSize.h , iconSize * camera.xScale, iconSize * camera.yScale };
			SDL_RenderFillRect(renderer, &icon);
		}
	}

	void zombiePositions() {
		SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
		for (auto zomb : *allZombies) {
			SDL_FRect dimen = zomb->getDimensions();
			float xProportion = dimen.x / worldWidth;
			float yProportion = dimen.y / worldHeight;

			SDL_FRect icon{ innerSize.x + xProportion * innerSize.w, innerSize.y + yProportion * innerSize.h , iconSize * camera.xScale, iconSize * camera.yScale };
			SDL_RenderFillRect(renderer, &icon);
		}
	}

private:
	SDL_FRect innerSize{0,0,0,0};
	SDL_FRect closeButton{ 0,0,0,0 };

	shared_ptr<vector<shared_ptr<GameObject>>> allObjects;
	shared_ptr<vector<shared_ptr<ZombieObj>>> allZombies;

	float scaleFactor = 13.0f / 15.0f;

	float iconSize = 5.0f;

	bool open = true;

	int sideLength = 300;
};