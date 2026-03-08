#pragma once
#include <SDL3/SDL.h>

#include "gameFiles/levelHandling/levelManager.h"
#include "gameFiles/UI/techTree/techTree.h"

enum gameState {
	Home,
	Level
};

class GameManager {
public:
	GameManager(SDL_Renderer* SDL_renderer): renderer(SDL_renderer){
		createHomeScreen();
	}
	~GameManager(){}

	void createHomeScreen() {
		SDL_Surface* surface = IMG_Load("draftArt/TitleScreen/homeScreen.png");
		if (!surface) {
			cerr << "Unable to load image! IMG_Error: " << SDL_GetError() << endl;
			return;
		}
		background = SDL_CreateTextureFromSurface(renderer, surface);
		SDL_SetTextureScaleMode(background, SDL_SCALEMODE_PIXELART);
		SDL_DestroySurface(surface); // Free the surface after creating the texture
		if (!background) {
			cerr << "Unable to create texture! SDL_Error: " << SDL_GetError() << endl;
			return;
		}

		string startButtonText = "Start";
		surface = TTF_RenderText_Solid(font, startButtonText.c_str(), startButtonText.length(), { 0,0,0,255 });
		startButton = SDL_CreateTextureFromSurface(renderer, surface);
	}

	void startLevel() {
		lManager = new LevelManager(renderer);
		currGameState = Level;
	}

	void handleInput(SDL_Event event) {
		switch (currGameState)
		{
		case Home:
			if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN && event.button.button == SDL_BUTTON_LEFT) {
				if (checkStartClick()) {
					currGameState = Level;
					lManager = new LevelManager(renderer);
				}
			}
			break;
		case Level:
			lManager->handleInput(event);
			break;
		}
	}

	bool checkStartClick() {
		//get click positon
		float cx, cy;
		getUIScaledMousePos(&cx, &cy);
		if (cx < startButtonSize.x) return false;
		if (cx > startButtonSize.x + startButtonSize.w) return false;
		if (cy < startButtonSize.y) return false;
		if (cy > startButtonSize.y + startButtonSize.h) return false;
		return true;
	}

	void render() {
		switch (currGameState)
		{
		case Home:
			renderHomeScreen();
			break;
		case Level:
			lManager->render();
			if (!lManager->getDepotAlive()) {
				lManager->saveOnExit();
				delete lManager;
				lManager = nullptr;
				currGameState = Home;

				//rescale the home screen based off zoom
				backgroundSize.w = camera.dimen.w;
				backgroundSize.h = camera.dimen.h;

				startButtonSize.x = camera.dimen.w / 2 - camera.dimen.w / 10;
				startButtonSize.y = camera.dimen.h - camera.dimen.h / 10;
				startButtonSize.w = camera.dimen.w / 5;
				startButtonSize.h = camera.dimen.h / 10;
			}
			break;
		}		
	}

	void renderHomeScreen() {
		SDL_RenderTexture(renderer, background, NULL, &backgroundSize);
		SDL_SetRenderDrawColor(renderer, 205, 195, 157, 255);
		SDL_RenderFillRect(renderer, &startButtonSize);
		SDL_RenderTexture(renderer, startButton, NULL, &startButtonSize);
	}

	void saveOnExit() {
		if (lManager) {
			lManager->saveOnExit();
		}
	}

private:
	SDL_Renderer* renderer;

	SDL_FRect backgroundSize{ 0, 0, camera.dimen.w, camera.dimen.h };
	SDL_Texture* background;
	SDL_FRect startButtonSize{ camera.dimen.w/2 - camera.dimen.w/10, camera.dimen.h - camera.dimen.h/10, camera.dimen.w/5, camera.dimen.h/10 };
	SDL_Texture* startButton;

	LevelManager* lManager;

	int currGameState = Home;
};