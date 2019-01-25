#define _USE_MATH_DEFINES

#include "Renderer.h"
#include "InitShader.h"
#include "MeshModel.h"
#include "Utils.h"
#include "Vertex.h"
#include <imgui/imgui.h>
#include <vector>
#include <cmath>
#include <algorithm>
#include <iostream>
#include <glad/glad.h>

Renderer::Renderer(Shader& shader, Scene& scene, int viewportWidth, int viewportHeight, int viewportX, int viewportY) :
	scene(scene),
	triangleDrawer(TriangleDrawer(shader, viewportWidth, viewportHeight)),
	fogger(Fogger())
{
}

Renderer::~Renderer()
{
}

void Renderer::ClearBuffers()
{
	glm::vec4 clearColor = scene.GetClearColor();
	glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

/* Function to scan min/max of X and Y values of 3 different Points fast*/
XYBorders Renderer::minMax(const Point & A, const Point & B, const Point & C) const
{
	XYBorders out;
	out.minX = (float)std::min(A.X, std::min(B.X, C.X));
	out.minY = (float)std::min(A.Y, std::min(B.Y, C.Y));
	out.maxX = (float)std::max(A.X, std::max(B.X, C.X));
	out.maxY = (float)std::max(A.Y, std::max(B.Y, C.Y));

	return out;
}

void Renderer::Render()
{
	// Start counting runtime
	auto start = std::chrono::high_resolution_clock::now();
	demoTriangle();

	// Stop counting runtime
	auto finish = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> elapsed = finish - start;
	scene.SetRenderExecutionTime(elapsed.count());
	return;
}

void Renderer::drawModels()
{
	if (scene.GetModelCount() == 0) return;

	std::vector<std::shared_ptr<MeshModel>> models = scene.GetModelsVector();
	for (std::vector<std::shared_ptr<MeshModel>>::const_iterator iterator = models.cbegin(); iterator!= models.cend(); ++iterator)
	{
		auto& model = **iterator; // Dereferences to MeshModel

		triangleDrawer.SetModel(&model);
		triangleDrawer.DrawTriangles();
	}
}

void Renderer::demoTriangle()
{
	Face face = Face(
		std::vector<int>{1, 2, 3},
		std::vector<int>{1, 2, 3},
		std::vector<int>()
	);
	std::vector<Face> faces{ face };

	std::vector<glm::vec3> vertices{
		glm::vec3(-0.5f,-0.5f,0.0f),
		glm::vec3(0.5f,-0.5f,0.0f),
		glm::vec3(0.0f,0.5f,0.0f)
	};

	MeshModel model = MeshModel(faces, vertices);
	triangleDrawer.SetModel(&model);
	triangleDrawer.DrawTriangles();
}