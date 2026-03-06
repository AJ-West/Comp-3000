#pragma once
#include <SDL3/SDL.h>
#include <vector>
#include "gameFiles/useThroughout/usefulFunctions.h"
#include "gameFiles/useThroughout/variables.h"

using namespace std;

class dayCycle {
public:
	dayCycle(int win, vector<int> currentTime, vector<vector<int>> times, vector<int> quantities, vector<int> direction): winDay(win), dayTime(currentTime), swarmTimes(times), swarmQuantity(quantities), swarmDirection(direction) {};
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
		if (swarmTimes.size() > 0) { 
			checkForSwarm(day);
		}// avoids error is last swarm has been and gone
	}
	void updateFilter() {
		int filterTime = mag(dayTime[2] - 12);
		rgba[3] = filterTime * filterTime * 1.25;
	}

	void checkForSwarm(int time) {
		if (dayTime[time] > swarmTimes[0][time]) {
			spawnSwarm = true;
		}
		else if (dayTime[time] == swarmTimes[0][time] && time != second) {
			checkForSwarm(time - 1);
		}
		else if (dayTime[time] == swarmTimes[0][time] && time == second) {
			spawnSwarm = true;
		}
	}

	void swarmSpawned() {
		spawnSwarm = false;
		swarmTimes.erase(swarmTimes.begin());
		swarmQuantity.erase(swarmQuantity.begin());
		swarmDirection.erase(swarmDirection.begin());
	}

	void render(SDL_Renderer* renderer) {
		SDL_SetRenderDrawColor(renderer, rgba[0], rgba[1], rgba[2], rgba[3]);
		SDL_FRect screen{ camera.dimen.x, camera.dimen.y, camera.dimen.w, camera.dimen.h };
		SDL_RenderFillRect(renderer, &screen);
	}

	//getters
	vector<int> getDayTime() { return dayTime; }
	bool getSpawnSwarm() { return spawnSwarm; }
	int getSpawnQuantity() { return swarmQuantity[0]; }
	int getSpawnDirection() { return swarmDirection[0]; }

private:
	// second = 0, minute = 1, hour = 2, day = 3
	vector<int> dayTime{ 0,0,12,0 }; // start at midday
	vector<int> timeIncrements{ 10,1,1,1 }; // start at midday
	vector<float> rgba{ 0,0,0,100 };

	vector<vector<int>> swarmTimes;
	vector<int> swarmQuantity;
	vector<int> swarmDirection;

	bool spawnSwarm = false;

	int winDay;
};