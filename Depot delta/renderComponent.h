#pragma once
#include "GameObject.h"
#include <SDL3_image/SDL_image.h>

class renderComponent : public Component {// renderers the object
public:
	virtual void update(GameObject* owner) { // render the current frame
		SDL_FRect size = owner->getDimensions();
		SDL_RenderTexture(renderer, texture, NULL, &size);
	}

	SDL_Texture* loadTexture(const char* filePath) {
		SDL_Surface* surface = IMG_Load(filePath);
		if (!surface) {
			cerr << "Unable to load image! IMG_Error: " << SDL_GetError() << endl;
			return nullptr;
		}
		SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
		SDL_DestroySurface(surface); // Free the surface after creating the texture
		if (!texture) {
			cerr << "Unable to create texture! SDL_Error: " << SDL_GetError() << endl;
			return nullptr;
		}
		return texture;
	}

	renderComponent(GameObject* obj, SDL_Renderer* Crenderer, const char* filePath) : Component(obj), renderer(Crenderer){
		texture = loadTexture(filePath);
	};
	virtual ~renderComponent() {}
private:
	SDL_Renderer* renderer;
	SDL_Texture* texture;
};