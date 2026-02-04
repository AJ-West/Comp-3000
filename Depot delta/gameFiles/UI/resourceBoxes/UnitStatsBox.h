#pragma once
#include <iostream>

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>

#include "gameFiles/useThroughout/variables.h"
#include "gameFiles/entities/gameObject.h"
#include "gameFiles/components/renderComponent.h"
#include "gameFiles/components/resourceComponent.h"
#include "gameFiles/UI/UIElement.h"

class textInput;
class UnitObj;
class DepotObj;
class ConvoyObj;

class UnitStatsBox : public UIElement {
public:
	UnitStatsBox(GameObject* sUnit, SDL_FRect rSize) : UIElement(rSize), unit(sUnit) {
		unitTexture = unit->getComponent<renderComponent>()->getTexture();
		unitSize = { size.x + 2.0f * camera.xScale, size.y + 42.0f * camera.yScale, size.w-4.0f*camera.xScale, 232.0f * camera.yScale };

		string type = "";
		if (typeid(*unit).name() == typeid(UnitObj).name()) {
			type = "Unit: ";
		}
		else if (typeid(*unit).name() == typeid(DepotObj).name()) {
			type = "Depot: ";
		}
		else if (typeid(*unit).name() == typeid(ConvoyObj).name()) {
			type = "Convoy: ";
		}

		name = type + std::to_string(unit->getID());
		SDL_Surface* surface = TTF_RenderText_Solid(font, name.c_str(), name.length(), { 0,0,0,255 });
		nameTexture = SDL_CreateTextureFromSurface(renderer, surface);
		nameSize = { size.x + 2.0f * camera.xScale, size.y + 3.0f * camera.yScale, size.w - 4.0f * camera.xScale, 37.0f * camera.yScale };

		resources = unit->getComponent<resourceComponent>();
		personnel = "Personnel: " + std::to_string(resources->getResourcesCount(PERSONNEL));
		surface = TTF_RenderText_Solid(font, personnel.c_str(), personnel.length(), { 0,0,0,255 });
		personnelTexture = SDL_CreateTextureFromSurface(renderer, surface);
		personnelSize = { size.x + 41.0f * camera.xScale, size.y + 302.0f * camera.yScale, size.w - 44.0f * camera.xScale, 36.0f * camera.yScale };

		ammo = "Ammunition: " + std::to_string(resources->getResourcesCount(AMMUNITION));
		surface = TTF_RenderText_Solid(font, ammo.c_str(), ammo.length(), { 0,0,0,255 });
		ammoTexture = SDL_CreateTextureFromSurface(renderer, surface);
		ammoSize = { size.x + 41.0f * camera.xScale, size.y + 372.0f * camera.yScale, size.w - 44.0f * camera.xScale, 36.0f * camera.yScale };

		DoS = "Days of supply: " + std::to_string(resources->getResourcesCount(DOS));
		surface = TTF_RenderText_Solid(font, DoS.c_str(), DoS.length(), { 0,0,0,255 });
		DoSTexture = SDL_CreateTextureFromSurface(renderer, surface);
		DoSSize = { size.x + 41.0f * camera.xScale, size.y + 419.0f * camera.yScale, size.w - 44.0f * camera.xScale, 36.0f * camera.yScale };

		Fuel = "Fuel: " + std::to_string(resources->getResourcesCount(FUEL));
		surface = TTF_RenderText_Solid(font, Fuel.c_str(), Fuel.length(), { 0,0,0,255 });
		FuelTexture = SDL_CreateTextureFromSurface(renderer, surface);
		FuelSize = { size.x + 41.0f * camera.xScale, size.y + 468.0f * camera.yScale, size.w - 44.0f * camera.xScale, 36.0f * camera.yScale };

		Scrap = "Scrap: " + std::to_string(resources->getResourcesCount(SCRAP));
		surface = TTF_RenderText_Solid(font, Scrap.c_str(), Scrap.length(), { 0,0,0,255 });
		ScrapTexture = SDL_CreateTextureFromSurface(renderer, surface);
		ScrapSize = { size.x + 41.0f * camera.xScale, size.y + 517.0f * camera.yScale, size.w - 44.0f * camera.xScale, 36.0f * camera.yScale };
	}
	~UnitStatsBox(){}

	bool update(SDL_Keycode key) {
		return true;
	}

	void render(SDL_Renderer* renderer) {
		SDL_RenderTexture(renderer, unitTexture, NULL, &unitSize);
		SDL_RenderTexture(renderer, nameTexture, NULL, &nameSize);
		
		SDL_RenderTexture(renderer, personnelTexture, NULL, &personnelSize);
		SDL_RenderTexture(renderer, ammoTexture, NULL, &ammoSize);
		SDL_RenderTexture(renderer, DoSTexture, NULL, &DoSSize);
		SDL_RenderTexture(renderer, FuelTexture, NULL, &FuelSize);
		SDL_RenderTexture(renderer, ScrapTexture, NULL, &ScrapSize);
	}

protected:
	vector<UIElement*> elements;
private:
	GameObject* unit;
	SDL_Texture* unitTexture = nullptr;
	SDL_FRect unitSize;
	std::shared_ptr<resourceComponent> resources;

	string name = "";
	SDL_Texture* nameTexture;
	SDL_FRect nameSize;

	string personnel = "";
	SDL_Texture* personnelTexture;
	SDL_FRect personnelSize;

	string ammo = "";
	SDL_Texture* ammoTexture;
	SDL_FRect ammoSize;

	string DoS = "";
	SDL_Texture* DoSTexture;
	SDL_FRect DoSSize;

	string Fuel = "";
	SDL_Texture* FuelTexture;
	SDL_FRect FuelSize;

	string Scrap = "";
	SDL_Texture* ScrapTexture;
	SDL_FRect ScrapSize;
};