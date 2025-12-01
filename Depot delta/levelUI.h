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

	void createTransferBox() {
		box = new transferBox({ 390.0f / 1440.0f * camera.dimen.w, 360.0f / 960.0f * camera.dimen.h, 660.0f / 1440.0f * camera.dimen.w , 240.0f / 960.0f * camera.dimen.h });
	}

	bool checkClickInput() { // checks if clicked on an input box in the UI
		//get click positon
		float cx, cy;
		getScaledMousePos(&cx, &cy);
		if (box->checkClick(cx, cy)) {
			return box->findClickedElement(cx, cy);
		}
		return false;
	}

private:
	SDL_Texture* resourceHoverTexture;

	dayCycle* time;
	SDL_FRect timeSize{ 62.0f / 1440.0f * camera.dimen.w, 9.0f / 960.0f * camera.dimen.h , 66.0f / 1440.0f * camera.dimen.w , 30.0f / 960.0f * camera.dimen.h };

	transferBox* box = nullptr;
};