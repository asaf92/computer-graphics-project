#include "PixelPlacer.h"

PixelPlacer::PixelPlacer(int viewportWidth, int viewportHeight, float * colorBuffer, float * zBuffer) :
	viewportWidth(viewportWidth),
	viewportHeight(viewportHeight),
	colorBuffer(colorBuffer),
	zBuffer(zBuffer),
	maxZ(1000.0f) 
{}

void PixelPlacer::PutPixel(const int i, const int j, const glm::vec3 & color, const float z)
{
	if (i < 0) return; if (i >= viewportWidth) return;
	if (j < 0) return; if (j >= viewportHeight) return;
	// Debug code
	float zValue = zBuffer[INDEX(viewportWidth, i, j, 0)];
	auto r   = colorBuffer[INDEX(viewportWidth, i, j, 0)];
	auto g   = colorBuffer[INDEX(viewportWidth, i, j, 1)];
	auto b   = colorBuffer[INDEX(viewportWidth, i, j, 2)];

	if (zBuffer[INDEX(viewportWidth, i, j, 0)] < z) return;
	zBuffer    [INDEX(viewportWidth, i, j, 0)] = z;
	colorBuffer[INDEX(viewportWidth, i, j, 0)] = color.x;
	colorBuffer[INDEX(viewportWidth, i, j, 1)] = color.y;
	colorBuffer[INDEX(viewportWidth, i, j, 2)] = color.z;
}

void PixelPlacer::SetViewport(int _viewportWidth, int _viewportHeight)
{
	viewportWidth = _viewportWidth;
	viewportHeight = _viewportHeight;
}