#pragma once
#include <SDL3/SDL.h>
#include <vector>
#include "gameFiles/useThroughout/usefulFunctions.h"
#include "gameFiles/useThroughout/variables.h"

using namespace std;

class dayCycle {
public:
	dayCycle(int win): winDay(win) {};
	~dayCycle() {};

	void update() {
		increaseTime(second);
	}

	void increaseTime(int period) {
		dayTime[period] += timeIncrements[period];
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
		else {
			if (dayTime[day] >= winDay) { cout << "win"; }
		}
		if (period == hour) {
			updateFilter();
		}
	}
	void updateFilter() {
		int filterTime = mag(dayTime[2] - 12);
		rgba[3] = filterTime * filterTime * 1.25;
	}

	void render(SDL_Renderer* renderer) {
		SDL_SetRenderDrawColor(renderer, rgba[0], rgba[1], rgba[2], rgba[3]);
		SDL_FRect screen{ camera.dimen.x, camera.dimen.y, camera.dimen.w, camera.dimen.h };
		SDL_RenderFillRect(renderer, &screen);
	}

	//getters
	vector<int> getDayTime() { return dayTime; }

private:
	// second = 0, minute = 1, hour = 2, day = 3
	vector<int> dayTime{ 0,0,12,0 }; // start at midday
	vector<int> timeIncrements{ 10,1,1,1 }; // start at midday
	vector<float> rgba{ 0,0,0,100 };

	vector<vector<int>> swarmTimes;
	vector<int> swarmQuantity;

	int winDay;
};