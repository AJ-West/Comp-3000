#include "gameFiles/useThroughout/usefulFunctions.h"

Vec2 normalise(Vec2 vec) {
	float magnitude = sqrt(vec.x * vec.x + vec.y * vec.y);
	return { vec.x / magnitude, vec.y / magnitude };
}

float mag(float a) {
	return sqrt(a * a);
}

float mag(Vec2 vec) {
	return sqrt(vec.x * vec.x + vec.y * vec.y);
}

float getDistance(Vec2 a, Vec2 b) {
	float xDist = a.x - b.x;
	float yDist = a.y - b.y;
	return sqrt(xDist * xDist + yDist * yDist);
}

// gets mouse click position on map
void getMapScaledMousePos(float* x, float* y) {
	float mx, my;
	SDL_GetMouseState(&mx, &my);
	SDL_RenderCoordinatesFromWindow(renderer, mx, my, x, y); // gets the click relative to camera zoom/offset
	*x = *x + camera.dimen.x;
	*y = *y + camera.dimen.y;
}

void getUIScaledMousePos(float* x, float* y) {
	float mx, my;
	SDL_GetMouseState(&mx, &my);
	SDL_RenderCoordinatesFromWindow(renderer, mx, my, x, y);
	*x = *x;
	*y = *y;
}

float scaleText(string text) {
	return static_cast<float>(text.length()) / static_cast<float>(FONT_SIZE);
}

vector<SDL_Texture*> loadResourceTextures() {
	vector<SDL_Texture*> resourceTextures;
	vector<const char*> resourceFiles = {
		"draftArt/resources/Personnel.png",
		"draftArt/resources/Ammo.png",
		"draftArt/resources/DoS.png",
		"draftArt/resources/Fuel.png",
		"draftArt/resources/Scrap.png"
	};
	for (int i = 0; i < resourceFiles.size(); i++) {
		resourceTextures.push_back(loadTexture(resourceFiles[i]));
	}
	return resourceTextures;
}

SDL_Texture* loadTexture(const char* filename) {
	SDL_Surface* surface = IMG_Load(filename);
	if (!surface) {
		cerr << "Unable to load image! IMG_Error: " << SDL_GetError() << endl;
		return nullptr;
	}
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_SetTextureScaleMode(texture, SDL_SCALEMODE_PIXELART);
	SDL_DestroySurface(surface); // Free the surface after creating the texture
	if (!texture) {
		cerr << "Unable to create texture! SDL_Error: " << SDL_GetError() << endl;
		return nullptr;
	}
	return texture;
}

const char* pickRandomFile(vector<const char*> filenames) {
	int pos = rand() % filenames.size();
	return filenames[pos];
}

bool charToBool(const char* value) {
	return strcmp(value, "1") == 0;
}

#ifdef _DEBUG
bool isVfptrFreed(const void* obj) {
	if (!obj) return false; // Null check
	const void* vfptr = *reinterpret_cast<const void* const*>(obj);
	return reinterpret_cast<uintptr_t>(vfptr) == 0xDDDDDDDDDDDDDDDDULL;
}
#endif

/*
void getScaledMousePos(float* x, float* y) {
	float mx = 0, my = 0;
	SDL_GetMouseState(&mx, &my); // logical coords when logical presentation is active
	*x = (float)mx;
	*y = (float)my;
}*/
