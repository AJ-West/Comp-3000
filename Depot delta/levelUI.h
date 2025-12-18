#ifndef LEVELUI_HEADER
#define LEVELUI_HEADER

#include <iostream>
#include <string>

#include "UIHandler.h"
#include "dayCycle.h"


using namespace std;

class transferBox;
class GameObject;

class levelUI : public UIHandler {
public:
	levelUI(SDL_Renderer* renderer, const char* filePath, dayCycle* gameTime);
	~levelUI();

	void loadResourceHoverTexture();

	void renderResourceHover();

	void renderTime();

	void render();

	void textInput(SDL_Keycode key);

	void createTransferBox(GameObject* sUnit, GameObject* sConvoy);

	bool checkClickInput();

private:
	SDL_Texture* resourceHoverTexture;

	dayCycle* time;
	

	transferBox* box = nullptr;
};

#endif