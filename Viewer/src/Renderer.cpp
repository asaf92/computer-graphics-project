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
	normalMappingShader.loadShaders("vshader_normal.glsl", "fshader_normal.glsl");
	activeShader = &colorShader;
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

void Renderer::Render()
{
	// Start counting runtime
	auto start = std::chrono::high_resolution_clock::now();

	drawModels();
	drawLights();
	drawFloor();

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
		drawMeshModel(model);
	}
}

void Renderer::drawMeshModel(const MeshModel & model)
{
	auto& lights = scene.GetLightsVector();
	auto& activeLight = lights[0];
	auto modelToWorld = model.GetWorldTransformation();
	auto worldToView = activeCamera.GetViewMatrix();
	activeCamera.RenderProjectionMatrix();
	auto projectionMatrix = activeCamera.GetProjectionMatrix();
	int numberOfLights = (int)lights.size();
	Texture2D* bumpMap = nullptr;

	// Vertex shader params
	activeShader->use();
	activeShader->setUniform("model", modelToWorld);
	activeShader->setUniform("view", worldToView);
	activeShader->setUniform("projection", projectionMatrix);
	activeShader->setUniform("numberOfLights", numberOfLights);
	if (scene.GetUseBumpMapping())
	{
		bumpMap = model.GetBumpMap();
		activeShader->setUniformSampler("bumpMap", 1);
	}

	// Fragment shader params
	for (int i = 0; i != numberOfLights; i++)
	{
		string lightsColorArrayString = std::string("lightColors[" + std::to_string(i) + ']').c_str();
		string lightsLocationArrayString = std::string("lightsPositions[" + std::to_string(i) + ']').c_str();
		activeShader->setUniform(lightsColorArrayString.c_str(), lights[i]->GetColor());
		activeShader->setUniform(lightsLocationArrayString.c_str(), Utils::Vec3FromVec4(lights[i]->GetLocation()));
	}

	activeShader->setUniform("ambiantColor", model.GetAmbientColor());
	activeShader->setUniform("ambiantLighting", scene.GetAmbientLight());
	activeShader->setUniform("diffuseColor", model.GetDiffuseColor());
	activeShader->setUniform("specularColor", model.GetSpecularColor());
	activeShader->setUniform("shininess", model.GetShininess());
	activeShader->setUniform("cameraLocation", activeCamera.GetCameraLocation());
	activeShader->setUniform("useTextures", model.TextureLoaded());
	activeShader->setUniform("useToonShading", scene.GetToonShading());
	activeShader->setUniform("toonShadingLevels", scene.GetToonShadingLevels());

	triangleDrawer.SetModel(&model);
	if (scene.GetFillTriangles()) 
	{
		model.BindTextures();
		triangleDrawer.FillTriangles();
		model.UnbindTextures();
	}
	else triangleDrawer.DrawTriangles();
}

void Renderer::drawLights()
{
	if (!scene.GetDrawLights()) return;

	std::vector<LightSource*> lights = scene.GetLightsVector();
	int numberOfLights = (int)lights.size();
	if (numberOfLights == 0) return;

	for (std::vector<LightSource*>::const_iterator iterator = lights.cbegin(); iterator != lights.cend(); ++iterator)
	{
		auto& lightSource = **iterator; // Dereferences to LightSource
		auto& lightColor = lightSource.GetColor();
		auto modelToWorld = lightSource.GetWorldTransformation();
		auto worldToView = activeCamera.GetViewMatrix();
		activeCamera.RenderProjectionMatrix();
		auto projectionMatrix = activeCamera.GetProjectionMatrix();

		// Vertex shader params
		activeShader->use();
		activeShader->setUniform("model", modelToWorld);
		activeShader->setUniform("view", worldToView);
		activeShader->setUniform("projection", projectionMatrix);
		activeShader->setUniform("numberOfLights", 0);
		activeShader->setUniform("ambiantColor", lightColor);
		activeShader->setUniform("ambiantLighting", lightColor);
		activeShader->setUniform("cameraLocation", activeCamera.GetCameraLocation());
		triangleDrawer.SetModel(&lightSource);
		triangleDrawer.DrawTriangles();
		triangleDrawer.FillTriangles();
	}
}

void Renderer::drawFloor()
{
	if (!scene.GetShowFloor()) return;
	drawMeshModel(*scene.GetFloor());
}
