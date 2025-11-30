#include <iostream>
#include <string>

#include "UIHandler.h"
#include "dayCycle.h"

using namespace std;

class levelUI : public UIHandler {
public:
	levelUI(SDL_Renderer* renderer, const char* filePath, dayCycle* gameTime);
	~levelUI();

	void loadResourceHoverTexture();

	void renderResourceHover();

	void renderTime();

private:
	SDL_Texture* resourceHoverTexture;

	dayCycle* time;
	SDL_FRect timeSize{ 62.0f / 1440.0f * camera.dimen.w, 9.0f / 960.0f * camera.dimen.h , 66.0f / 1440.0f * camera.dimen.w , 30.0f / 960.0f * camera.dimen.h };
};