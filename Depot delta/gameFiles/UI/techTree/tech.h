#pragma once

#include <unordered_map>

enum techStatus{
	affordable,
	unaffordable,
	unlocked,
	locked,
	purchased
};

enum techType{
	modifier,
	unlock
};

class Tech : public UIElement{
public:
	//Tech(int tCost, SDL_FRect size, string tName, string tKeyName) : UIElement(size), cost(tCost), name(tName), keyName(tKeyName) {}
	Tech(int tCost, SDL_FRect size, string tName, string desc) : UIElement(size), cost(tCost), name(tName), description(desc) {
		SDL_Surface* surface = TTF_RenderText_Solid(font, desc.c_str(), desc.length(), { 0,0,0,255 });
		descriptionTexture = SDL_CreateTextureFromSurface(renderer, surface);
		tSize = { size.x - size.w*2, size.y+size.h, size.w * 5, size.h};
	}
	~Tech(){}

	void unlock(int money){
		if (money < cost) { status = unaffordable; }
		else { status = affordable; }
	}

	void onBuy() {
		status = purchased;
		effect();
	}

	void render(SDL_Renderer* renderer) {
		switch (status) {
		case affordable:
			SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
			break;
		case unaffordable:
			SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
			break;
		case locked:
			SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
			break;
		case purchased:
			SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
			break;
		}
		SDL_RenderFillRect(renderer, &size);
	}

	void renderHover(SDL_Renderer* renderer) {
		SDL_RenderTexture(renderer, descriptionTexture, NULL, &tSize);
	}

	void effect() {}

	//setters
	void setStatus(int sStatus) { status = sStatus; }

	//getters
	int getStatus() { return status; }
	int getCost() { return cost; }

private:
	string name;
	int id;
	int status = locked;
	int type = modifier;
	float modifyValue = 0.0f;

	string description;
	SDL_Texture* descriptionTexture;
	SDL_FRect tSize;

	int purchaseAmount = 0;
	int boughtAmount = 0;

	//string keyName;

	int cost;
};