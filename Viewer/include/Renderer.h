#pragma once
#include "Scene.h"
#include "Line.h"
#include "XYBorders.h"
#include "Shader.h"
#include "PixelPlacer.h"
#include "TriangleDrawer.h"
#include "Fogger.h"
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
	Shader& shader;

	// Buffers
	void createBuffers(int viewportWidth, int viewportHeight);
	float *colorBuffer;
	float *zBuffer;
	static const float maxZ;
	bool zBufferChanged;

	// Viewport
	int viewportWidth;
	int viewportHeight;
	int viewportX;
	int viewportY;

	// Fogger
	Fogger fogger;

	// Drawing
	TriangleDrawer triangleDrawer;
	PixelPlacer pixelPlacer;
	void putPixel(int x, int y, const glm::vec3& color,float z);
	void drawLine(Line& line);
	void drawLine(Line & line, const glm::vec3 & color);
	void drawTriangle(const Point& PointA, const Point& PointB, const Point& PointC);
	void drawAxis(const glm::mat4 & projectionMatrix, const glm::mat4 & viewMatrix);
	void drawStraightLine( int &y0,  int &y1,  int x0,  int x1, glm::vec3 & lineColor);

	// Calculations
	XYBorders minMax(const Point& A, const Point& B, const Point& C) const;
	float CalcWOneValue(const Point & A, const Point & B, const Point & C, int y, int x);
	float CalcWTwoValue(const Point & A, const Point & B, const Point & C, int y, float w1);
	Point toScreenPixel(const Point& point) const;

	GLuint glScreenTex;
	GLuint glScreenVtc;

	void createOpenGLBuffer();
	void initOpenGLRendering();
	int RenderExecutionTime;

	// Render methods
	void drawFog();
	void drawModels(glm::vec4 &activeCameraLocation, const glm::mat4 & projectionMatrix, const glm::mat4 & viewMatrix);
	void drawFace(std::vector<Face>::iterator &facesIterator, std::vector<glm::vec3> & vertices, const glm::mat4x4 & worldTransform, const std::vector<glm::vec3> & normals, const glm::mat4 & projectionMatrix, const glm::mat4 & viewMatrix, glm::mat4x4 &transformMatrix);
	void drawLightSources(const std::vector<LightSource*>& lightsVector);
	void draw3DLine(glm::vec4 PointA, glm::vec4 PointB, const glm::mat4x4 & projectionMatrix, const glm::mat4x4 & viewMatrix);
	void draw3DLine(glm::vec4 PointA, glm::vec4 PointB, const glm::mat4x4 & projectionMatrix, const glm::mat4x4 & viewMatrix, const glm::vec3 & color);

public:
	Renderer(Shader& shader, Scene& scene, int viewportWidth, int viewportHeight, int viewportX = 0, int viewportY = 0);
	~Renderer();

	void Render();
	void SwapBuffers();
	void ClearColorBuffer(const glm::vec3& color);
	void ClearZBuffer();
	void SetViewport(int viewportWidth, int viewportHeight, int viewportX = 0, int viewportY = 0);

};
