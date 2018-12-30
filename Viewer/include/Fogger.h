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
	glm::vec3 fogColor;

	// calculations
	float getFogFactor(float zValue);
	
public:
	Fogger() : fogColor(0.5f,0.5f,0.5f) {}
	void SetColorBuffer(float* buffer) { colorBuffer = buffer; }
	void SetZBuffer(float* buffer) { zBuffer = buffer; }
	void SetStart(float value) { start = value; }
	void SetFinish(float value) { finish = value; }
	void SetViewport(int width, int height) { viewportWidth = width; viewportHeight = height; }

	float  GetStart	  () const { return start;}
	float  GetFinish  () const { return finish;}

	void AddFog(); // Main API call
};