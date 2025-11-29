#include <iostream>
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>

#include "UIHandler.h"

using namespace std;

class levelUI : public UIHandler {
public:
	levelUI(SDL_Renderer* renderer, const char* filePath) : UIHandler(renderer, filePath) {}
	~levelUI();

};