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

Renderer::Renderer(Scene& scene) : scene(scene), activeCamera(scene.GetActiveCamera()), triangleDrawer(TriangleDrawer()), fogger(Fogger()) 
{ 
	colorShader.loadShaders("vshader_color.glsl", "fshader_color.glsl"); 
	createDemoTriangle();
}

Renderer::~Renderer()
{
	delete demoTriangleModel;
}



void Renderer::ClearBuffers()
{
	glm::vec4 clearColor = scene.GetClearColor();
	glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::Render()
{
	// Start counting runtime
	auto start = std::chrono::high_resolution_clock::now();

	demoTriangle();
	drawModels();

	// Stop counting runtime
	auto finish = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> elapsed = finish - start;
	scene.SetRenderExecutionTime(elapsed.count());
	return;
}

void Renderer::drawModels()
{
	if (scene.GetModelCount() == 0) return;

	std::vector<MeshModel*> models = scene.GetModelsVector();
	for (std::vector<MeshModel*>::const_iterator iterator = models.cbegin(); iterator!= models.cend(); ++iterator)
	{
		auto& model = **iterator; // Dereferences to MeshModel
		auto& lights = scene.GetLightsVector();
		auto& activeLight = lights[0];
		auto modelToWorld = model.GetWorldTransformation();
		auto worldToView = activeCamera.GetViewMatrix();
		activeCamera.RenderProjectionMatrix();
		auto projectionMatrix = activeCamera.GetProjectionMatrix();
		
		// Vertex shader params
		colorShader.use();
		colorShader.setUniform("model", modelToWorld );
		colorShader.setUniform("view", worldToView);
		colorShader.setUniform("projection", projectionMatrix);

		// Fragment shader params
		colorShader.setUniform("ambiantColor", model.GetAmbientColor());
		colorShader.setUniform("ambiantLighting", scene.GetAmbientLight());
		colorShader.setUniform("lightColor", activeLight->GetColor());
		colorShader.setUniform("diffuseColor", model.GetDiffuseColor());
		colorShader.setUniform("specularColor", model.GetSpecularColor());
		colorShader.setUniform("lightSourceLocation", Utils::Vec3FromVec4(*activeLight->GetLocation()));
		colorShader.setUniform("shininess", model.GetShininess());
		colorShader.setUniform("cameraLocation", activeCamera.GetCameraLocation());
		triangleDrawer.SetModel(&model);
		triangleDrawer.DrawTriangles();
		if(scene.GetFillTriangles()) triangleDrawer.FillTriangles();
		//lightShader.use();
		//lightShader.setUniform("model", modelToWorld);
		//lightShader.setUniform("view", worldToView);
		//lightShader.setUniform("projection", projectionMatrix);
		//if (scene.GetFillTriangles()) triangleDrawer.FillTriangles();
	}
}

/*
* Demo to succesfully display a MeshModel with two faces
* to form a star of david
*/
void Renderer::demoTriangle()
{
	if (!scene.GetDemoTriangles()) return;

	colorShader.use();
	auto modelToWorld =     glm::mat4(1.0f);
	modelToWorld[1][1] = 0.2f;
	modelToWorld[0][0] = 0.15f;
	auto worldToView =      glm::mat4(1.0f);
	auto projectionMatrix = glm::mat4(1.0f);
	colorShader.setUniform("model", modelToWorld);
	colorShader.setUniform("view", worldToView);
	colorShader.setUniform("projection", projectionMatrix);
	lightShader.use();
	triangleDrawer.SetModel(demoTriangleModel);
	triangleDrawer.DrawTriangles();
	if(scene.GetFillTriangles()) triangleDrawer.FillTriangles();
}

void Renderer::createDemoTriangle()
{
	Face face = Face(
		std::vector<int>{1, 2, 3},
		std::vector<int>{1, 2, 3},
		std::vector<int>()
	);
	Face secondFace = Face(
		std::vector<int>{4, 5, 6},
		std::vector<int>{4, 5, 6},
		std::vector<int>()
	);
	std::vector<Face> faces{ face, secondFace };

	std::vector<glm::vec3> vertices{
		glm::vec3(-0.5f,-0.5f,0.0f),
		glm::vec3(0.5f,-0.5f,0.0f),
		glm::vec3(0.0f,0.75f,0.0f),
		glm::vec3(-0.5f,0.5f,0.0f),
		glm::vec3(0.5f,0.5f,0.0f),
		glm::vec3(0.0f,-0.75f,0.0f),

	};

	demoTriangleModel = new MeshModel(faces, vertices, "Demo Triangle");
}
