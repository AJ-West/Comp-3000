#pragma once
#include <iostream>
#include "variables.h"

#ifndef USEFULFUNCTIONS_H
#define USEFULFUNCTIONS_H

using namespace std;

pair<float, float> normalise(pair<float, float> vector);

float mag(float a);

void getScaledMousePos(float* x, float* y);

#endif