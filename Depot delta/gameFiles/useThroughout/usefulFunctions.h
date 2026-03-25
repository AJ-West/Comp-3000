#pragma once
#include <iostream>
#include "gameFiles/useThroughout/variables.h"
#include <SDL3_image/SDL_image.h>
#include <vector>
#include <string>
#include <filesystem>

#ifndef USEFULFUNCTIONS_H
#define USEFULFUNCTIONS_H

using namespace std;

Vec2 normalise(Vec2 vec);

float mag(float a);
float mag(Vec2 vec);

float getDistance(Vec2 a, Vec2 b);

void getMapScaledMousePos(float* x, float* y);
void getUIScaledMousePos(float* x, float* y);

float scaleText(string text);

vector<SDL_Texture*> loadResourceTextures();

SDL_Texture* loadTexture(const char* filename);

const char* pickRandomFile(vector<const char*> filename);

bool charToBool(const char* value);

#ifdef _DEBUG
bool isVfptrFreed(const void* obj);
#endif

#endif