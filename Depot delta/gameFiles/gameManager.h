#pragma once
#include <SDL3/SDL.h>

#include <iostream>
#include <fstream>

#include "gameFiles/levelHandling/levelManager.h"
#include "gameFiles/UI/techTree/techTree.h"

enum gameState {
	Home,
	LevelSelect,
	Level
};

enum levels {
	NOLVL,
	TUTORIAL,
	LEVEL1,
	LEVEL2,
	LEVEL3,
	CONTINUE,
};

class GameManager {
public:
	GameManager(SDL_Renderer* SDL_renderer): renderer(SDL_renderer){
		createHomeScreen();
	}
	~GameManager(){}

	void createHomeScreen() {
		background = loadTexture("draftArt/TitleScreen/homeScreen.png");

		string startButtonText = "Start";
		SDL_Surface* surface = TTF_RenderText_Solid(font, startButtonText.c_str(), startButtonText.length(), { 0,0,0,255 });
		startButton = SDL_CreateTextureFromSurface(renderer, surface);
	}

	void createLevelSelectScreen() {
		background = loadTexture("draftArt/TitleScreen/homeScreen.png");

		string text = "Tutorial";
		SDL_Surface* surface = TTF_RenderText_Solid(font, text.c_str(), text.length(), { 0,0,0,255 });
		tutButton = SDL_CreateTextureFromSurface(renderer, surface);

		text = "Level 1";
		surface = TTF_RenderText_Solid(font, text.c_str(), text.length(), { 0,0,0,255 });
		lvl1Button = SDL_CreateTextureFromSurface(renderer, surface);

		text = "Level 2";
		surface = TTF_RenderText_Solid(font, text.c_str(), text.length(), { 0,0,0,255 });
		lvl2Button = SDL_CreateTextureFromSurface(renderer, surface);

		text = "Level 3";
		surface = TTF_RenderText_Solid(font, text.c_str(), text.length(), { 0,0,0,255 });
		lvl3Button = SDL_CreateTextureFromSurface(renderer, surface);

		text = "Continue";
		surface = TTF_RenderText_Solid(font, text.c_str(), text.length(), { 0,0,0,255 });
		conButton = SDL_CreateTextureFromSurface(renderer, surface);
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
				if (checkClick(startButtonSize)) {
					currGameState = LevelSelect;
					createLevelSelectScreen();
				}
			}
			break;
		case LevelSelect:
			if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN && event.button.button == SDL_BUTTON_LEFT) {
				int clicked = checkLevelClick();
				if (clicked != NOLVL) {
					currGameState = Level;
					switch (clicked) {
					case TUTORIAL:
						copyFile("maps/tutorial.xml");
						break;
					case LEVEL1:
						copyFile("maps/level1.xml");
						break;
					case LEVEL2:
						copyFile("maps/level2.xml");
						break;
					case LEVEL3:
						copyFile("maps/level3.xml");
						break;
					}
					lManager = new LevelManager(renderer);
				}
			}
			break;
		case Level:
			lManager->handleInput(event);
			break;
		}
	}

	bool checkClick(SDL_FRect button) {
		//get click positon
		float cx, cy;
		getUIScaledMousePos(&cx, &cy);
		if (cx < button.x) return false;
		if (cx > button.x + button.w) return false;
		if (cy < button.y) return false;
		if (cy > button.y + button.h) return false;
		return true;
	}

	int checkLevelClick() { // identifies if a level was selected which level
		if (checkClick(tutButtonSize)) return TUTORIAL;
		else if (checkClick(lvl1ButtonSize)) return LEVEL1; 
		else if (checkClick(lvl2ButtonSize)) return LEVEL2; 
		else if (checkClick(lvl3ButtonSize)) return LEVEL3; 
		else if (checkClick(conButtonSize)) return CONTINUE; 
		return NOLVL;
	}

	void render() {
		switch (currGameState)
		{
		case Home:
			renderHomeScreen();
			break;
		case LevelSelect:
			renderLevelSelect();
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

	void renderLevelSelect() {
		SDL_RenderTexture(renderer, background, NULL, &backgroundSize);
		SDL_SetRenderDrawColor(renderer, 205, 195, 157, 255);

		SDL_RenderFillRect(renderer, &tutButtonSize);
		SDL_RenderTexture(renderer, tutButton, NULL, &tutButtonSize);

		SDL_RenderFillRect(renderer, &lvl1ButtonSize);
		SDL_RenderTexture(renderer, lvl1Button, NULL, &lvl1ButtonSize);

		SDL_RenderFillRect(renderer, &lvl2ButtonSize);
		SDL_RenderTexture(renderer, lvl2Button, NULL, &lvl2ButtonSize);

		SDL_RenderFillRect(renderer, &lvl3ButtonSize);
		SDL_RenderTexture(renderer, lvl3Button, NULL, &lvl3ButtonSize);

		SDL_RenderFillRect(renderer, &conButtonSize);
		SDL_RenderTexture(renderer, conButton, NULL, &conButtonSize);
	}

	bool copyFile(const std::string& src) {
		std::ifstream in(src, std::ios::binary);
		std::ofstream out("maps/active.xml", std::ios::binary);

		if (!in) {
			std::cerr << "Error: Cannot open source file: " << src << "\n";
			return false;
		}
		if (!out) {
			std::cerr << "Error: Cannot open destination file: maps/active.xml" << "\n";
			return false;
		}

		out << in.rdbuf(); // Copy contents
		return true;
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

	// Home screen
	SDL_FRect startButtonSize{ camera.dimen.w/2 - camera.dimen.w/10, camera.dimen.h - camera.dimen.h/10, camera.dimen.w/5, camera.dimen.h/10 };
	SDL_Texture* startButton;

	// Level select screen
	SDL_FRect tutButtonSize{ camera.dimen.w / 3 - camera.dimen.w / 10, camera.dimen.h/3 - camera.dimen.h / 10, camera.dimen.w / 5, camera.dimen.h / 10 }; // tutorial
	SDL_Texture* tutButton;

	SDL_FRect lvl1ButtonSize{ camera.dimen.w / 4 - camera.dimen.w / 10, 2*camera.dimen.h/3 - camera.dimen.h / 10, camera.dimen.w / 5, camera.dimen.h / 10 }; // level1
	SDL_Texture* lvl1Button;

	SDL_FRect lvl2ButtonSize{ 2*camera.dimen.w / 4 - camera.dimen.w / 10, 2*camera.dimen.h/3 - camera.dimen.h / 10, camera.dimen.w / 5, camera.dimen.h / 10 }; // level2
	SDL_Texture* lvl2Button;

	SDL_FRect lvl3ButtonSize{ 3*camera.dimen.w / 4 - camera.dimen.w / 10, 2*camera.dimen.h/3 - camera.dimen.h / 10, camera.dimen.w / 5, camera.dimen.h / 10 }; // level3
	SDL_Texture* lvl3Button;

	SDL_FRect conButtonSize{ 2*camera.dimen.w / 3 - camera.dimen.w / 10, camera.dimen.h/3 - camera.dimen.h / 10, camera.dimen.w / 5, camera.dimen.h / 10 }; // continue
	SDL_Texture* conButton;


	LevelManager* lManager;

	int currGameState = Home;
};