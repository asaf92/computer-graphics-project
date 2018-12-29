#pragma once
#include"PixelPlacer.h"
#include "Point.h"
#include "Line.h"
#include "Utils.h"
#include "XYBorders.h"
#include <glm/glm.hpp>
#include <algorithm> // For std::min

#define EPSILON 0.001f

struct BycentricCoordinates;

class TriangleDrawer
{
private:
	// Dependencies
	Shader& shader;
	PixelPlacer& pixelPlacer;
	int viewportWidth;
	int viewportHeight;

	// Screen Points
	glm::vec3 screenPointA;
	glm::vec3 screenPointB;
	glm::vec3 screenPointC;

	// World Points
	glm::vec4 unscaledPointA;
	glm::vec4 unscaledPointB;
	glm::vec4 unscaledPointC;

	// Private methods
	void drawStraightLine(glm::vec3 pointA, glm::vec4 pointB);
	XYBorders minMax() const;
	bool pointInTriangle(int _x, int _y);
	BycentricCoordinates getBycentricCoordinates(int _x, int _y);
	bool allPointsAreInFrame();
public:
	// Constructors 
	TriangleDrawer(Shader& shader, PixelPlacer& pixelPlacer, int viewportWidth, int viewportHeight);

	// Getters

	// Setters
	void SetUnscaledPoints(const Point& pointA, const Point& pointB, const Point& pointC);

	// Methods
	void DrawTriangle();
	void SetViewport(int viewportWidth, int viewportHeight);
};

struct BycentricCoordinates
{
	float w1;	// A
	float w2;   // B
	float w3;   // C
};