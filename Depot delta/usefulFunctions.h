#pragma once
#include <iostream>

using namespace std;

pair<float, float> normalise(pair<float, float> vector) {
	float magnitude = sqrt(vector.first * vector.first + vector.second * vector.second);
	return { vector.first / magnitude, vector.second / magnitude };
}

float mag(float a) {
	return sqrt(a * a);
}