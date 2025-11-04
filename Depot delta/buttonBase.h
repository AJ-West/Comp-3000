#pragma once
#include <iostream>
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <functional>
#include <string>

using namespace std;

class BaseButton {
public:
	BaseButton() {}
	~BaseButton() {}

	bool checkClick() {return false;}
	void onClick() {
		if (func)
			func();
		else
			level(num);
	}
	void checkHover(int x, int y) {
		isHover = isWithinRect(x, y);
	}

	bool isWithinRect(int x, int y) {
		if (x < rect.x) return false;
		if (x > rect.x + rect.w) return false;
		if (y < rect.y) return false;
		if (y > rect.y + rect.h) return false;
		return true;
	}

	void render() {
		SDL_RenderTexture(renderer, texture, NULL, &rect);
	}

	//setters
	void setIsDisplayed(bool display) { isDisplayed = display; }
	void setFunc(function<void()> function) { func = function; }
	void setLevel(function<void(int)> levelLoad, int number) { level = levelLoad; num = number; }

	//getters
	bool getIsDisplayed() { return isDisplayed; }
	bool getIsHover() { return isHover; }

private:
	bool isDisplayed;
	bool isHover = false;

	function<void()> func;
	function<void(int)> level;
	int num;

	SDL_Texture* texture;
	SDL_FRect rect;

	SDL_Renderer* renderer;

	string name;
};