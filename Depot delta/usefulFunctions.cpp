#include "usefulFunctions.h"

pair<float, float> normalise(pair<float, float> vector) {
	float magnitude = sqrt(vector.first * vector.first + vector.second * vector.second);
	return { vector.first / magnitude, vector.second / magnitude };
}

float mag(float a) {
	return sqrt(a * a);
}

void getScaledMousePos(float* x, float* y) {
	float mx, my;
	SDL_GetMouseState(&mx, &my);
	SDL_RenderCoordinatesFromWindow(renderer, mx, my, x, y);
}