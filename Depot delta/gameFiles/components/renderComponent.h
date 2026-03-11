#pragma once
#include "gameFiles/entities/gameObject.h"
#include <SDL3_image/SDL_image.h>

class renderComponent : public Component {// renderers the object
public:
	virtual void update(GameObject* owner) { // render the current frame
		SDL_FRect size = owner->getDimensions();
		size.x -= camera.dimen.x;
		size.y -= camera.dimen.y;
		SDL_RenderTexture(renderer, texture, NULL, &size);
		if (owner->getSelected()) {
			SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
			SDL_RenderRect(renderer, &size);
		}
		if (owner->getAttacking()) {
			SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
			SDL_RenderRect(renderer, &size);
		}
		if (!owner->getAlive()) {
			SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
			SDL_RenderRect(renderer, &size);
		}
	}

	SDL_Texture* getTexture() { return texture; }

	renderComponent(GameObject* obj, SDL_Renderer* Crenderer, const char* filePath) : Component(obj), renderer(Crenderer){
		texture = loadTexture(filePath);
	};
	virtual ~renderComponent() {}
private:
	SDL_Renderer* renderer;
	SDL_Texture* texture;
};