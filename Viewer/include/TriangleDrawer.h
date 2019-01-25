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
	int verticesNumber;
	GLuint vao;

public:
	// Setters
	void SetModel(const IMeshObject * const _model);

	// Methods
	void DrawTriangles() const;

};

