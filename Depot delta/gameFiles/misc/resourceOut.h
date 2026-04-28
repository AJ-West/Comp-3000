#pragma once

#include "gameFiles/useThroughout/variables.h"
#include "gameFiles/entities/gameObject.h"

class ResourceOut {
public:
	ResourceOut(GameObject* attachedObject, int resourceType): object(attachedObject), type(resourceType) {
		spawnTime = frameStart;
		texturePos.x = type * 24 ;

		texture = loadTexture("draftArt/EmptyResourceIcons.png");
	}

	void update(SDL_Renderer* renderer) {
		SDL_FRect objectDimen = object->getDimensions();
		calcPosition(objectDimen);
		render(renderer, objectDimen);
		checkLife();
	}

	void calcPosition(SDL_FRect objectDimen) {
		dimen.x = objectDimen.x + objectDimen.w/2 - dimen.w/2;
		if (objectDimen.y < worldHeight / 2) {
			dimen.y = objectDimen.y + objectDimen.h + dimen.h/2;
		}
		else {
			dimen.y = objectDimen.y - 3*dimen.h / 2;
		}
		ensureInView();
	}

	void ensureInView() { // unique responses to identify what side(s) the object it outside of the view
		if (dimen.x < camera.dimen.x) dimen.x = camera.dimen.x;
		if (dimen.x > camera.dimen.x + camera.dimen.w) dimen.x = camera.dimen.x + camera.dimen.w - dimen.w;
		if (dimen.y < camera.dimen.y) dimen.y = camera.dimen.y;
		if (dimen.y > camera.dimen.y + camera.dimen.h) dimen.y = camera.dimen.y + camera.dimen.h - dimen.h;
	}

	void render(SDL_Renderer* renderer, SDL_FRect objectDimen) {
		dimen.x -= camera.dimen.x;
		dimen.y -= camera.dimen.y;
		SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
		SDL_RenderLine(renderer, dimen.x + dimen.w / 2, dimen.y + dimen.h / 2, objectDimen.x + objectDimen.w/2 - camera.dimen.x, objectDimen.y + objectDimen.h / 2 - camera.dimen.y);
		SDL_RenderTexture(renderer, texture, &texturePos, &dimen);
		
	}

	void checkLife() {
		if (frameStart - spawnTime >= lifeTime) {
			alive = false;
		}
	}

	//getters
	bool getAlive() { return alive; }

private:
	Uint32 spawnTime;

	bool alive = true;

	int lifeTime = 7500;

	int type;

	GameObject* object;

	SDL_Texture* texture;

	SDL_FRect dimen{ 0,0,48,48 };
	SDL_FRect texturePos{ 0,0,24,24 };
};