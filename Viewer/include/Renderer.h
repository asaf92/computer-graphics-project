#pragma once
#include "Scene.h"
#include "Line.h"
#include "XYBorders.h"
#include <vector>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include <imgui/imgui.h>


/*
 * Renderer class.
 *
 * Made by Asaf Agami 2018
 */
class Renderer
{
private:
	Scene& scene;
	float *colorBuffer;
	float **zBuffer;
	int viewportWidth;
	int viewportHeight;
	int viewportX;
	int viewportY;
	const float maxZ = 10000.0f;

	void putPixel(int x, int y, const glm::vec3& color);
	void putPixel(const int x, const int y, float z, const glm::vec3& color);
	void createBuffers(int viewportWidth, int viewportHeight);
	void drawLine(Line& line);
	void drawLine(Line & line, const glm::vec3 & color);
	XYBorders minMax(const Point& A, const Point& B, const Point& C) const;
	void fillTriangle(const Point & PointA, const Point & PointB, const Point & PointC, const XYBorders & borders, const glm::vec3 color);
	float CalcWOneValue(const Point & A, const Point & B, const Point & C, int y, int x);
	float CalcWTwoValue(const Point & A, const Point & B, const Point & C, int y, float w1);
	void drawTriangle(const Point& PointA, const Point& PointB, const Point& PointC) { drawTriangle(PointA,PointB,PointC,glm::vec3(0)); }
	void drawTriangle(const Point& PointA, const Point& PointB, const Point& PointC, const glm::vec3 color);

	GLuint glScreenTex;
	GLuint glScreenVtc;

	void createOpenGLBuffer();
	void initOpenGLRendering();
	void setXZeroToBeSmaller(Line & line,  int &x0,  int &y0,  int &x1,  int &y1);
	void drawStraightLine( int &y0,  int &y1,  int x0,  int x1, glm::vec3 & lineColor);
	Point toScreenPixel(const Point& point) const;
	void drawAxis(const glm::mat4 & projectionMatrix, const glm::mat4 & viewMatrix);
	void clearZBuffer();

public:
	Renderer(Scene& scene, int viewportWidth, int viewportHeight, int viewportX = 0, int viewportY = 0);
	~Renderer();

	void Render();
	void SwapBuffers();
	void ClearColorBuffer(const glm::vec3& color);
	void SetViewport(int viewportWidth, int viewportHeight, int viewportX = 0, int viewportY = 0);

	void draw3DLine(glm::vec4 PointA, glm::vec4 PointB, const glm::mat4x4 & projectionMatrix, const glm::mat4x4 & viewMatrix);
	void draw3DLine(glm::vec4 PointA, glm::vec4 PointB, const glm::mat4x4 & projectionMatrix, const glm::mat4x4 & viewMatrix, const glm::vec3 & color);

	// Add more methods/functionality as needed...
};

