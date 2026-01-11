#include "usefulFunctions.h"

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

void getScaledMousePos(float* x, float* y) {
	float mx, my;
	SDL_GetMouseState(&mx, &my);
	SDL_RenderCoordinatesFromWindow(renderer, mx, my, x, y);
	*x = *x + camera.dimen.x;
	*y = *y + camera.dimen.y;
}
/*
void getScaledMousePos(float* x, float* y) {
	float mx = 0, my = 0;
	SDL_GetMouseState(&mx, &my); // logical coords when logical presentation is active
	*x = (float)mx;
	*y = (float)my;
}*/
