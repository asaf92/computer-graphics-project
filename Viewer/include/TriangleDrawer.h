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
	int viewportWidth;
	int viewportHeight;

	// OpenGL era
	// Vertices
	IMeshObject const* model;
	std::vector<Vertex> vertices;
	int verticesNumber;
	GLuint vao;

public:
	// Constructors 
	TriangleDrawer(Shader & _shader, int _viewportWidth, int _viewportHeight);

	// Setters
	void SetModel(const IMeshObject * const _model);
	void SetVao(GLuint _vao) { vao = _vao; }
	void SetVerticesNumber(int _size) { verticesNumber = _size; }

	// Methods
	void DrawTriangles() const;
	void SetViewport(int viewportWidth, int viewportHeight);

};

