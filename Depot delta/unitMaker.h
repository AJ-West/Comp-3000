#pragma once

#include "resourceBox.h"

class textInput;

class unitMaker : public ResourceBox {
public:
	unitMaker(SDL_FRect rSize) : ResourceBox(rSize) {
		//add make unit button
		elements.push_back(new newUnitButton({ size.x + 74.0f / 1440.0f * camera.dimen.w, size.y + 140.0f / 960.0f * camera.dimen.h, 192.0f / 1440.0f * camera.dimen.w , 48.0f / 960.0f * camera.dimen.h }, true));
		//add cancel button
		elements.push_back(new newUnitButton({ size.x + 394.0f / 1440.0f * camera.dimen.w, size.y + 140.0f / 960.0f * camera.dimen.h, 192.0f / 1440.0f * camera.dimen.w , 48.0f / 960.0f * camera.dimen.h }, false));
	}
	~unitMaker() {}	
};