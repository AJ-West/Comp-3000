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
		switch (key)
		{
		case SDLK_0:
			box->updateText("0");
			break;
		case SDLK_1:
			box->updateText("1");
			break;
		case SDLK_2:
			box->updateText("2");
			break;
		case SDLK_3:
			box->updateText("3");
			break;
		case SDLK_4:
			box->updateText("4");
			break;
		case SDLK_5:
			box->updateText("5");
			break;
		case SDLK_6:
			box->updateText("6");
			break;
		case SDLK_7:
			box->updateText("7");
			break;
		case SDLK_8:
			box->updateText("8");
			break;
		case SDLK_9:
			box->updateText("9");
			break;
		case SDLK_BACKSPACE:
			box->updateText("b");
			break;

		}
	}

	void createTransferBox() {
		box = new transferBox();
	}

	bool checkClickInput() { // checks if clicked on an input box in the UI
		//get click positon
		float cx, cy;
		getScaledMousePos(&cx, &cy);
		if (box->checkClick(cx, cy)) {
			return true;
		}
		return false;
	}

private:
	SDL_Texture* resourceHoverTexture;

	dayCycle* time;
	SDL_FRect timeSize{ 62.0f / 1440.0f * camera.dimen.w, 9.0f / 960.0f * camera.dimen.h , 66.0f / 1440.0f * camera.dimen.w , 30.0f / 960.0f * camera.dimen.h };

	transferBox* box = nullptr;
};