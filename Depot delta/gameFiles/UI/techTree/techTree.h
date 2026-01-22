#pragma once
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <iostream>
#include <cstdlib>

#include "gameFiles/UI/techTree/tech.h"
#include "vector"

using namespace std;

enum currentTech {
	depotT,
	unitT,
	convoyT,
	sizeOfT
};

class TechTree {
public:
	TechTree(SDL_Renderer* renderer, LevelManager* lManager): manager(lManager) {
		readTechFile();
		// Load resource hover texture
		SDL_Surface* surface = IMG_Load("techTree/art/blueprint.png");
		if (!surface) {
			cerr << "Unable to load image! IMG_Error: " << SDL_GetError() << endl;
			return;
		}
		background = SDL_CreateTextureFromSurface(renderer, surface);
		//SDL_SetTextureScaleMode(background, SDL_SCALEMODE_PIXELART);
		SDL_DestroySurface(surface); // Free the surface after creating the texture
		if (!background) {
			cerr << "Unable to create texture! SDL_Error: " << SDL_GetError() << endl;
			return;
		}
	}
	~TechTree(){
		saveTechFile();
	}

	void readTechFile() {
		for (int i = 0; i < 15; i++) {
			div_t divV = div(i, 5);
			SDL_FRect pos{0,0,0,0};
			pos.w = camera.dimen.w / 24;
			pos.h = pos.w;
			pos.x = camera.dimen.w/7 * (divV.rem+1) + camera.dimen.w / 14 - pos.w/2;
			pos.y = camera.dimen.h / 3 * divV.quot + camera.dimen.h/6 - pos.h/2;

			depotTechs.emplace_back(new Tech(200, pos));
		}
	}

	void saveTechFile() {

	}

	void render(SDL_Renderer* renderer) {
		SDL_RenderTexture(renderer, background, NULL, &camera.dimen); // render background over whole screen
		for (auto techBox : depotTechs) {
			techBox->render(renderer);
		}
		/*switch (current) {
		case depotT:
			for (auto techBox : depotTechs) {
				techBox->render();
			}
			break;
		case unitT:
			for (auto techBox : unitTechs) {
				techBox->render();
			}
			break;
		case convoyT:
			for (auto techBox : convoyTechs) {
				techBox->render();
			}
			break;
		}*/
	}

	void nextArrowClick() {
		current += 1;
		if (current >= sizeOfT) {
			current = 0;
		}
	}

	void prevArrowClick() {
		current -= 1;
		if (current < 0) {
			current = sizeOfT-1;
		}
	}

	void unPause() { manager->setPaused(false); }

private:
	int current = depotT;

	SDL_Texture* background;

	vector<Tech*> depotTechs;
	vector<Tech*> unitTechs;
	vector<Tech*> convoyTechs;

	LevelManager* manager;
};