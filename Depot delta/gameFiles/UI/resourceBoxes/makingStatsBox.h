#pragma once

#pragma once
#include <iostream>

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>

#include "gameFiles/useThroughout/variables.h"
#include "gameFiles/entities/gameObject.h"
#include "gameFiles/UI/UIElement.h"
#include "gameFiles/useThroughout/stats/unitStats.h"
#include "gameFiles/useThroughout/stats/convoyStats.h"

class MakingStatsBox : public UIElement {
public:
	MakingStatsBox(SDL_FRect rSize) : UIElement(rSize) {
		updateArt(basicUnit);
	}
	~MakingStatsBox() {}

	bool update(SDL_Keycode key) {
		return true;
	}

	void updateArt(int typeIndex) {
		FONT_SIZE = 30;
		switch (typeIndex) {
		case basicUnit:
			makeBasicUnit();
			break;
		case basicConvoy:
			makeBasicConvoy();
			break;
		}
		FONT_SIZE = 20;
	}

	void makeBasicUnit() {
		updateName(basicUnit);
		updateImage(unitStats.art);
		updatePersonnel(unitStats.rMax[PERSONNEL]);
		updateAmmunition(unitStats.rMax[AMMUNITION]);
		updateDOS(unitStats.rMax[DOS]);
		updateFuel(unitStats.rMax[FUEL]);
		updateScrap(unitStats.rMax[SCRAP]);
	}

	void makeBasicConvoy() {
		updateName(basicConvoy);
		updateImage(convoyStats.art);
		updatePersonnel(convoyStats.rMax[PERSONNEL]);
		updateAmmunition(convoyStats.rMax[AMMUNITION]);
		updateDOS(convoyStats.rMax[DOS]);
		updateFuel(convoyStats.rMax[FUEL]);
		updateScrap(convoyStats.rMax[SCRAP]);
	}

	void updateName(int typeIndex) {
		name = unlockedUnit.names[typeIndex];
		surface = TTF_RenderText_Solid(font, name.c_str(), name.length(), { 0,0,0,255 });
		nameTexture = SDL_CreateTextureFromSurface(renderer, surface);
		nameSize = { size.x + 4.0f * camera.xScale, size.y + 3.0f * camera.yScale, (size.w - 8.0f * camera.xScale) * scaleText(name), 39.0f * camera.yScale };
	}
	
	void updateImage(const char* filename) {
		surface = IMG_Load(filename);
		if (!surface) {
			cerr << "Unable to load image! IMG_Error: " << SDL_GetError() << endl;
			return;
		}
		imageTexture = SDL_CreateTextureFromSurface(renderer, surface);
		SDL_SetTextureScaleMode(imageTexture, SDL_SCALEMODE_PIXELART);
		SDL_DestroySurface(surface); // Free the surface after creating the texture
		if (!imageTexture) {
			cerr << "Unable to create texture! SDL_Error: " << SDL_GetError() << endl;
			return;
		}
		imageSize = { size.x + 4.0f * camera.xScale, size.y + 45.0f * camera.yScale, (size.w - 8.0f * camera.xScale) * scaleText(name), 249.0f * camera.yScale };
	}

	void updatePersonnel(int max) {
		personnel = "Personnel Max: " + std::to_string(max);
		surface = TTF_RenderText_Solid(font, personnel.c_str(), personnel.length(), { 0,0,0,255 });
		personnelTexture = SDL_CreateTextureFromSurface(renderer, surface);
		personnelSize = { size.x + 45.0f * camera.xScale, size.y + 312.0f * camera.yScale, (size.w - 60.0f * camera.xScale) * scaleText(personnel), 36.0f * camera.yScale };
	}

	void updateAmmunition(int max) {
		ammo = "Ammunition Max: " + std::to_string(max);
		surface = TTF_RenderText_Solid(font, ammo.c_str(), ammo.length(), { 0,0,0,255 });
		ammoTexture = SDL_CreateTextureFromSurface(renderer, surface);
		ammoSize = { size.x + 45.0f * camera.xScale, size.y + 366.0f * camera.yScale, (size.w - 60.0f * camera.xScale) * scaleText(ammo), 36.0f * camera.yScale };
	}

	void updateDOS(int max) {
		DoS = "Days of supply Max: " + std::to_string(max);
		surface = TTF_RenderText_Solid(font, DoS.c_str(), DoS.length(), { 0,0,0,255 });
		DoSTexture = SDL_CreateTextureFromSurface(renderer, surface);
		DoSSize = { size.x + 45.0f * camera.xScale, size.y + 413.0f * camera.yScale, (size.w - 60.0f * camera.xScale) * scaleText(DoS), 36.0f * camera.yScale };
	}

	void updateFuel(int max) {
		Fuel = "Fuel Max: " + std::to_string(max);
		surface = TTF_RenderText_Solid(font, Fuel.c_str(), Fuel.length(), { 0,0,0,255 });
		FuelTexture = SDL_CreateTextureFromSurface(renderer, surface);
		FuelSize = { size.x + 45.0f * camera.xScale, size.y + 465.0f * camera.yScale, (size.w - 60.0f * camera.xScale) * scaleText(Fuel), 36.0f * camera.yScale };
	}

	void updateScrap(int max) {
		Scrap = "Scrap Max: " + std::to_string(max);
		surface = TTF_RenderText_Solid(font, Scrap.c_str(), Scrap.length(), { 0,0,0,255 });
		ScrapTexture = SDL_CreateTextureFromSurface(renderer, surface);
		ScrapSize = { size.x + 45.0f * camera.xScale, size.y + 519.0f * camera.yScale, (size.w - 60.0f * camera.xScale) * scaleText(Scrap), 36.0f * camera.yScale };
	}

	void render(SDL_Renderer* renderer) {		
		SDL_RenderTexture(renderer, nameTexture, NULL, &nameSize);
		SDL_RenderTexture(renderer, imageTexture, NULL, &imageSize);
		SDL_RenderTexture(renderer, personnelTexture, NULL, &personnelSize);
		SDL_RenderTexture(renderer, ammoTexture, NULL, &ammoSize);
		SDL_RenderTexture(renderer, DoSTexture, NULL, &DoSSize);
		SDL_RenderTexture(renderer, FuelTexture, NULL, &FuelSize);
		SDL_RenderTexture(renderer, ScrapTexture, NULL, &ScrapSize);
	}

private:
	SDL_Surface* surface;

	string name = "";
	SDL_Texture* nameTexture;
	SDL_FRect nameSize;

	string image;
	SDL_Texture* imageTexture = nullptr;
	SDL_FRect imageSize;

	string personnel = "";
	SDL_Texture* personnelTexture;
	SDL_FRect personnelSize{ size.x + 41.0f * camera.xScale, size.y + 302.0f * camera.yScale, (size.w - 44.0f * camera.xScale) * scaleText(personnel), 36.0f * camera.yScale };

	string ammo = "";
	SDL_Texture* ammoTexture;
	SDL_FRect ammoSize{ size.x + 41.0f * camera.xScale, size.y + 350.0f * camera.yScale, (size.w - 44.0f * camera.xScale) * scaleText(ammo), 36.0f * camera.yScale };

	string DoS = "";
	SDL_Texture* DoSTexture;
	SDL_FRect DoSSize{ size.x + 41.0f * camera.xScale, size.y + 398.0f * camera.yScale, (size.w - 44.0f * camera.xScale) * scaleText(DoS), 36.0f * camera.yScale };

	string Fuel = "";
	SDL_Texture* FuelTexture;
	SDL_FRect FuelSize{ size.x + 41.0f * camera.xScale, size.y + 446.0f * camera.yScale, (size.w - 44.0f * camera.xScale) * scaleText(Fuel), 36.0f * camera.yScale };

	string Scrap = "";
	SDL_Texture* ScrapTexture;
	SDL_FRect ScrapSize{ size.x + 41.0f * camera.xScale, size.y + 494.0f * camera.yScale, (size.w - 44.0f * camera.xScale) * scaleText(Scrap), 36.0f * camera.yScale };
};