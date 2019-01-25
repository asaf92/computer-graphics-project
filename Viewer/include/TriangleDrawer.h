#pragma once
#include"PixelPlacer.h"
#include "Point.h"
#include "Line.h"
#include "Utils.h"
#include "XYBorders.h"
#include "BycentricCoordinates.h"
#include <glm/glm.hpp>
#include <algorithm> // For std::min
#include "Vertex.h"

#define EPSILON 0.001f
#define NUM_VERTICES 3

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
	XYBorders minMax() const;
	bool pointInTriangle(int _x, int _y);
	bool pointInTriangle(const BycentricCoordinates & bycentricCoords) const;
	BycentricCoordinates getBycentricCoordinates(int _x, int _y);
	bool allPointsAreInFrame();

	// OpenGL era
	// Vertices
	IMeshObject const* model;
	std::vector<Vertex> vertices;
	int verticesNumber;
	GLuint vao;

public:
	// Constructors 
	TriangleDrawer(Shader& shader, PixelPlacer& pixelPlacer, int viewportWidth, int viewportHeight);

	// Setters
	void SetModel(const IMeshObject * const _model);
	void SetVao(GLuint _vao) { vao = _vao; }
	void SetVerticesNumber(int _size) { verticesNumber = _size; }

	// Methods
	void DrawTriangles() const;
	void DrawTriangle();
	void SetViewport(int viewportWidth, int viewportHeight);

	// OpenGL era
};

