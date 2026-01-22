#pragma once

enum techStatus{
	affordable,
	unaffordable,
	unlocked,
	locked,
	purchased
};

class Tech {
public:
	Tech(int tCost, SDL_FRect iSize) : cost(tCost), size(iSize) {}
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
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderFillRect(renderer, &size);
	}

	void effect() {}

private:
	int status = locked;

	int cost;

	SDL_FRect size;
};