#pragma once
#include <iostream>
#include "variables.h"

#ifndef USEFULFUNCTIONS_H
#define USEFULFUNCTIONS_H

using namespace std;

Vec2 normalise(Vec2 vec);

float mag(float a);
float mag(Vec2 vec);

float getDistance(Vec2 a, Vec2 b);

void getScaledMousePos(float* x, float* y);

#endif