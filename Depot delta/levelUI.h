#ifndef LEVELUI_HEADER
#define LEVELUI_HEADER

#include <iostream>
#include <string>

#include "UIHandler.h"
#include "dayCycle.h"
#include "transferBox.h"

using namespace std;

class levelUI : public UIHandler {
public:
	levelUI(SDL_Renderer* renderer, const char* filePath, dayCycle* gameTime);
	~levelUI();

	void loadResourceHoverTexture();

	void renderResourceHover();

	void renderTime();

	void render() {
		if (box) {
			box->render(renderer);
		}
		SDL_FRect size{ 0, 0, camera.dimen.w, camera.dimen.h };
		SDL_RenderTexture(renderer, texture, NULL, &size);
	}

	void textInput(SDL_Keycode key) {
		box->update(key);
	}

	void createTransferBox(GameObject* sUnit, GameObject* sConvoy) {
		box = new transferBox({ 390.0f / 1440.0f * camera.dimen.w, 360.0f / 960.0f * camera.dimen.h, 660.0f / 1440.0f * camera.dimen.w , 240.0f / 960.0f * camera.dimen.h }, sUnit, sConvoy);
	}

	bool checkClickInput() { // checks if clicked on an input box in the UI
		//get click positon
		float cx, cy;
		getScaledMousePos(&cx, &cy);
		if (box) {
			if (box->checkClick(cx, cy)) {
				bool clicked = box->findClickedElement(cx, cy);
				if (clicked && box->getToDelete()) {
					delete box;
					box = nullptr;
				}
				return clicked;
			}
		}
		return false;
	}

private:
	SDL_Texture* resourceHoverTexture;

	dayCycle* time;
	

	transferBox* box = nullptr;
};

#endif