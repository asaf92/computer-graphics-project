#pragma once
#include <glm/glm.hpp>

#define INDEX(width,x,y,c) ((x)+(y)*(width))*3+(c)

class PixelPlacer
{
private:
	// Dependencies
	int viewportWidth;
	int viewportHeight;
	float* colorBuffer;
	float* zBuffer;
	const float maxZ;
public:
	PixelPlacer(int viewportWidth, int viewportHeight, float* colorBuffer, float* zBuffer): 
		viewportWidth(viewportWidth),
		viewportHeight(viewportHeight),
		colorBuffer(colorBuffer),
		zBuffer(zBuffer),
	    maxZ(1000.0f){}

	void PutPixel(const int i, const int j, const glm::vec3& color, const float z);
};