#pragma once
#include <SDL3/SDL.h>
#include <vector>
#include "usefulFunctions.h"
#include "variables.h"

using namespace std;

enum time {second, minute, hour, day};

class dayCycle {
public:
	dayCycle() {};
	~dayCycle() {};

	void update() {
		increaseTime(second);
	}

	void increaseTime(int period) {
		dayTime[period] += 1;
		if (period != day) {
			if (dayTime[period] == 60 && period != hour) {
				dayTime[period] = 0;
				increaseTime(period + 1);
			}
			else if (dayTime[period] == 24 && period == hour) {
				dayTime[period] = 0;
				increaseTime(period + 1);
			}
		}
		if (period == hour) {
			updateFilter();
		}
	}
	void updateFilter() {
		int filterTime = mag(dayTime[2] - 12);
		cout << filterTime << '\n';
		rgba[3] = filterTime * filterTime * 1.25;
	}

	void render(SDL_Renderer* renderer) {
		SDL_SetRenderDrawColor(renderer, rgba[0], rgba[1], rgba[2], rgba[3]);
		SDL_FRect screen{ camera.x, camera.y, camera.width, camera.height };
		SDL_RenderFillRect(renderer, &screen);
	}

private:
	vector<int> dayTime{ 0,0,12,0 }; // start at midday
	vector<float> rgba{ 0,0,0,100 };
};