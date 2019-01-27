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
	Fogger fogger;
	Camera& activeCamera;
	IMeshObject* demoTriangleModel;

	// Drawing
	TriangleDrawer triangleDrawer;


	// Shaders
	ShaderProgram colorShader;
	ShaderProgram lightShader;

	// Calculations
	XYBorders minMax(const Point& A, const Point& B, const Point& C) const;

	// Render methods
	void drawModels();
	void demoTriangle();

	void createDemoTriangle();

public:
	Renderer(Scene& scene);
	~Renderer();

	void Render();
	void ClearBuffers();
	
};
