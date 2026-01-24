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
	Tech(int tCost, SDL_FRect size, string tName) : UIElement(size), cost(tCost), name(tName) {}
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

	int purchaseAmount = 0;
	int boughtAmount = 0;

	//string keyName;

	int cost;
};