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

		outline = loadTexture("art/UI/level/minimap.png");
		closed = loadTexture("art/UI/level/closeMap.png");
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
		else {
			size.y = size.y + 3*size.h/4;
			size.w = size.w/4;
			size.h = size.h/4;
		}
	}

	void updateInnerSize() {
		innerSize.x = size.x + size.w / 9;
		innerSize.y = size.y + size.h / 9;
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
			SDL_RenderTexture(renderer, outline, NULL, &size);		
			calculateMapPositions();
			renderCameraPos();
		}
		else {
			SDL_RenderTexture(renderer, closed, NULL, &size);
		}
	}

	void calculateMapPositions() {
		//thread objs(&Minimap::objectPositions, this);
		//thread zombs(&Minimap::zombiePositions, this);

		//objs.join();
		//zombs.join();
		objectPositions();
		zombiePositions();
	}

	void objectPositions() {		
		for (auto obj : *allObjects) {
#ifdef _DEBUG // needs removing and fixing the issue by replacing .get with native shared ptrs to avoid vfptr = 0xddddd
			if (!isVfptrFreed(obj.get())) {
				if (obj->getType() == HUMAN) {
					if (obj->getUnitOrConvoy() == UNIT) {
						SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
					}
					else {
						SDL_SetRenderDrawColor(renderer, 255, 150, 0, 255);
					}
				}
				else if (obj->getType() == BUILDING && obj->getAlive()) {
					SDL_SetRenderDrawColor(renderer, 0, 150, 255, 255);
				}
				else if (obj->getType() == BUILDING) {
					SDL_SetRenderDrawColor(renderer, 150, 150, 255, 255);
				}
				else if (obj->getType() == DEPOT) {
					SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
				}


				SDL_FRect dimen = obj->getDimensions();
				float xProportion = dimen.x / worldWidth;
				float yProportion = dimen.y / worldHeight;

				float wProportion = dimen.w / worldWidth;
				float hProportion = dimen.h / worldHeight;

				SDL_FRect icon{ innerSize.x + xProportion * innerSize.w, innerSize.y + yProportion * innerSize.h , wProportion * innerSize.w, hProportion * innerSize.h };
				SDL_RenderFillRect(renderer, &icon);
			}
#endif
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

	void renderCameraPos() {
		SDL_FRect relCamPos{ camera.dimen.x / worldWidth, camera.dimen.y / worldHeight, camera.dimen.w / worldWidth, camera.dimen.h / worldHeight };
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_FRect icon{ innerSize.x + relCamPos.x * innerSize.w, innerSize.y + relCamPos.y * innerSize.h , relCamPos.w * innerSize.w, relCamPos.h * innerSize.h };
		SDL_RenderRect(renderer, &icon);
	}

	void toggle() {	open = !open; }

	bool checkForToggle(float x, float y) {
		if (open) {
			return checkCloseClick(x, y);
		}
		else {
			return checkClick(x, y);
		}
	}

	bool checkCloseClick(float x, float y) {
		if (x < closeButton.x) return false;
		if (x > closeButton.x + closeButton.w) return false;
		if (y < closeButton.y) return false;
		if (y > closeButton.y + closeButton.h) return false;
		return true;
	}

private:
	SDL_FRect innerSize{0,0,0,0};
	SDL_FRect closeButton{ 0,0,0,0 };

	SDL_Texture* outline;
	SDL_Texture* closed;

	shared_ptr<vector<shared_ptr<GameObject>>> allObjects;
	shared_ptr<vector<shared_ptr<ZombieObj>>> allZombies;

	float scaleFactor = 7.0f / 9.0f;

	float iconSize = 5.0f;

	bool open = true;

	int sideLength = 300;
};