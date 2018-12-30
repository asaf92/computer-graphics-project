#pragma once

#include <algorithm>
#include "Scene.h"

#define INDEX(width,x,y,c) ((x)+(y)*(width))*3+(c)

class Fogger
{
private:
	// Dependencies 
	float* colorBuffer;
	float* zBuffer;
	int viewportWidth;
	int viewportHeight;

	// Properties
	float start;
	float finish;

	// calculations
	float getFogFactor(float zValue);
	
public:
	void SetColorBuffer(float* buffer) { colorBuffer = buffer; }
	void SetZBuffer(float* buffer) { zBuffer = buffer; }
	void SetStart(float value) { start = value; }
	void SetFinish(float value) { finish = value; }

	void AddFog(); // Main API call
};