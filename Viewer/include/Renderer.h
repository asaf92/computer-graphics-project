#pragma once
#include "Scene.h"
#include "Line.h"
#include "XYBorders.h"
#include "Shader.h"
#include "PixelPlacer.h"
#include "TriangleDrawer.h"
#include "Fogger.h"
#include "ShaderProgram.h"
#include <vector>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include <imgui/imgui.h>
#include <chrono>

/*
 * Renderer class.
 *
 * Made by Asaf Agami 2018
 */
class Renderer
{
private:
	// Depndencies
	Scene& scene;

	// Viewport
	int viewportWidth;
	int viewportHeight;
	int viewportX;
	int viewportY;

	// Fogger
	Fogger fogger;

	// Drawing
	TriangleDrawer triangleDrawer;

	// Calculations
	XYBorders minMax(const Point& A, const Point& B, const Point& C) const;

	// Render methods
	void drawModels();
	void demoTriangle();

	// Shaders
	ShaderProgram colorShader;

public:
	Renderer(Shader& shader, Scene& scene, int viewportWidth, int viewportHeight, int viewportX = 0, int viewportY = 0);
	~Renderer();

	void Render();
	void ClearBuffers();
	
};
