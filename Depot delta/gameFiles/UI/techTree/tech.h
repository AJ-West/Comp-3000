#pragma once

#include <cmath>
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
enum listNames {
	depotUpgrades,
	unitUpgrades,
	convoyUpgrades
};

class Tech : public UIElement{
public:
	Tech(int tCost, SDL_FRect size, string tName, string desc, string tKeyName, int bList, SDL_Texture* allIcons) : UIElement(size), cost(tCost), name(tName), description(desc), keyName(tKeyName), listBelong(bList), Icons(allIcons) {}
	~Tech(){}

	void render(SDL_Renderer* renderer) {
		switch (status) {
		case affordable:
			SDL_SetRenderDrawColor(renderer, boughtAmount*25, 255, boughtAmount * 25, 25);
			break;
		case unaffordable:
			SDL_SetRenderDrawColor(renderer, 255, 0, 0, 25);
			break;
		case locked:
			SDL_SetRenderDrawColor(renderer, 0, 0, 0, 125);
			break;
		case purchased:
			SDL_SetRenderDrawColor(renderer, 0, 0, 100, 25);
			break;
		}
		SDL_RenderTexture(renderer, Icons, &loc, &size);
		SDL_RenderFillRect(renderer, &size);
	}

	void renderHover(SDL_Renderer* renderer) {
		SDL_RenderTexture(renderer, descriptionTexture, NULL, &tSize);
	}

	bool buy() { // when clicked
		if (status == affordable) {
			effect();
			return true;
		}
		else if (status == unaffordable) { std::cout << "unaffordable"; } // need to update to inform player
		return false;
	}

	void increaseCost(){ cost += cost; }

	void effect() {
		switch (type)
		{
		case modifier:
			modifierFunc();
			break;
		case unlock:
			unlockFunc();
			break;
		}	
		boughtAmount++;
		if (boughtAmount == purchaseAmount) { status = purchased; }
	}

	void modifierFunc() { // used for upgrading existing stats
		switch (listBelong)
		{
		case depotUpgrades:
			depotTechVal[keyName] *= modifyValue;
			break;
		case unitUpgrades:
			unitTechVal[keyName] *= modifyValue;
			break;
		case convoyUpgrades:
			convoyTechVal[keyName] *= modifyValue;
			break;
		}
	}

	void unlockFunc() { // used for unlocking a feature e.g. new unit type

	}

	void updateListValue() {
		switch (listBelong)
		{
		case depotUpgrades:
			depotTechVal[keyName] = std::pow(modifyValue, boughtAmount);
			break;
		case unitUpgrades:
			unitTechVal[keyName] = std::pow(modifyValue, boughtAmount);
			break;
		case convoyUpgrades:
			convoyTechVal[keyName] = std::pow(modifyValue, boughtAmount);
			break;
		}
	}

	void calcLoc() {
		loc.x = id * 32 - 32;
		loc.y = listBelong * 32;
	}

	void updateDescription() {
		costDescription = to_string(boughtAmount) + "/" + to_string(purchaseAmount) + " Cost " + to_string(cost) + ": " + description;
		SDL_Surface* surface = TTF_RenderText_Solid(font, costDescription.c_str(), costDescription.length(), { 0,0,0,255 });
		descriptionTexture = SDL_CreateTextureFromSurface(renderer, surface);
		tSize = { size.x - size.w * 2, size.y + size.h, size.w * 5, size.h };
	}

	//setters
	void setID(int i) { id = i; }
	void setStatus(int sStatus) { status = sStatus; }
	void setType(int ty) { type = ty; }
	void setModifyValue(float m) { modifyValue = m; }
	void setPurchaseAmount(int p) { purchaseAmount = p; }
	void setBoughtAmount(int b) { 
		boughtAmount = b; 
		updateListValue();
	}

	//getters
	int getStatus() { return status; }
	int getCost() { return cost; }
	int getID() { return id; }
	int getPA() { return purchaseAmount; }
	int getBA() { return boughtAmount; }
	int getType() { return type; }
	string getKeyName() { return keyName; }

private:
	int listBelong;

	string name;
	int id;
	int status = locked;
	int type = modifier;
	float modifyValue = 0.0f;

	string description;
	string costDescription = "";
	SDL_Texture* descriptionTexture;
	SDL_FRect tSize;
	SDL_FRect loc = { 0,0,32,32 };

	SDL_Texture* Icons;

	int purchaseAmount = 0;
	int boughtAmount = 0;

	string keyName;

	int cost;
};