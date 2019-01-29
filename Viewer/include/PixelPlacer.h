#pragma once
#include <glm/glm.hpp>

#define INDEX(width,x,y,c) ((x)+(y)*(width))*3+(c)

class PixelPlacer
{
private:
	// Dependencies
	float* colorBuffer;
	float* zBuffer;

	// Properties
	int viewportWidth;
	int viewportHeight;

	// Constants
	const float maxZ;
public:
	PixelPlacer(int viewportWidth, int viewportHeight, float* colorBuffer, float* zBuffer);
	void PutPixel(const int i, const int j, const glm::vec3& color, const float z);


	// Setters
	void SetViewport(int viewportWidth, int viewportHeight);
	void SetColorBuffer(float* _colorBuffer) { colorBuffer = _colorBuffer; }
	void SetZBuffer(float* _zBuffer) { zBuffer = _zBuffer; }
	void SetViewportWidth(int width) { viewportWidth = width; }
	void SetViewportHeight(int height) { viewportHeight = height; }
};