#include <iostream>

#include "UIHandler.h"

using namespace std;

class levelUI : public UIHandler {
public:
	levelUI(SDL_Renderer* renderer, const char* filePath);
	~levelUI();

	void loadResourceHoverTexture();

	void renderResourceHover();

private:
	SDL_Texture* resourceHoverTexture;
};