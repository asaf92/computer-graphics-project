#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>
#include <string>
#include <memory>
#include <chrono>
#include "MeshModel.h"
#include "Camera.h"
#include "ProjectionType.h"
#include "PointLightSource.h"
#include "ParallelLightSource.h"
#include "ShadingModels.h"

#define MAX_LIGHTS_NUMBER 8

/*
 * Scene class.
 * This class holds all the scene information (models, cameras, lights, etc..)
 */

class Scene {
private:
	std::vector<MeshModel*> models;
	std::vector<Camera> cameras;
	std::vector<LightSource*> lights;
	glm::vec4 ambientLight;

	// Indices
	int activeCameraIndex;
	int activeModelIndex;
	int activeLightsIndex;

	// Execution Times
	double imGuiRenderExecutionTime;
	double zBufferExecutionTime;
	double colorBufferExecutionTime;
	double swapBuffersExecutionTime;
	double renderExecutionTime;

	// Booleans
	bool showNormals = false;
	bool fillTriangles = true;
	bool drawAxis = false;
	bool demoTriangles = false;
	bool showFloor = false;
	bool fogEnabled = false;

	// Fog
	float fogStart;
	float fogFinish;

	// Shader
	bool useBumpMapping = false;

	// Lights
	bool showingLights;

	// World
	float worldRadius;
	glm::vec4 clearColor;

	// Toon Shading
	bool toonShading = false;
	int toonShadingLevels = 1;

	// Generic models
	Cube floor;

public:
	Scene();
	~Scene();

	// World
	const float GetWorldRadius() const    { return worldRadius; }
	void SetWorldRadius(const float size) { worldRadius = size; }
	const glm::vec4 GetClearColor() const { return clearColor; }
	void SetClearColor(const glm::vec4 color) { clearColor = color; }

	// Floor
	const MeshModel* GetFloor() { return &floor; }

	// Models
	void AddModel(MeshModel * const model);
	const std::vector<MeshModel*> GetModelsVector() const { return models; };
	MeshModel* GetActiveModel() const;
	const int GetModelCount() const;
	void SetActiveModelIndex(const int index);
	const int GetActiveModelIndex() const;

	// Cameras
	void AddCamera(const Camera& camera);
	const std::vector<Camera>& GetCamerasVector() const;
	const int GetCameraCount() const;
	void SetActiveCameraIndex(const int index);
	const int GetActiveCameraIndex() const;
	Camera& GetActiveCamera() { return cameras[GetActiveCameraIndex()]; }

	// Normals
	void SetShowNormals(const bool value) { showNormals = value; }
	bool GetShowNormals() const { return showNormals; }

	// Booleans
	void SetFillTriangles(const bool value) { fillTriangles = value; }
	bool GetFillTriangles() const { return fillTriangles; }
	void SetDrawAxis(const bool value) { drawAxis = value; }
	bool GetDrawAxis() const { return drawAxis; }
	void SetDemoTriangles(const bool value) { demoTriangles = value; }
	bool GetDemoTriangles() { return demoTriangles; }
	void SetShowFloor(const bool value) { showFloor = value; }
	bool GetShowFloor() { return showFloor; }

	// Stats
	const double GetRenderExecutionTime() const { return renderExecutionTime; }
	void SetRenderExecutionTime(double time) { renderExecutionTime = time; }
	const double GetImGuiRenderExecutionTime() const { return imGuiRenderExecutionTime; }
	void SetImGuiRenderExecutionTime(double time) { imGuiRenderExecutionTime = time; }
	const double GetZBufferExecutionTime() const { return zBufferExecutionTime; }
	void SetZBufferExecutionTime(double time) { zBufferExecutionTime = time; }
	const double GetColorBufferExecutionTime() const { return colorBufferExecutionTime; }
	void SetColorBufferExecutionTime(double time) { colorBufferExecutionTime = time; }
	const double GetSwapBuffersExecutionTime() const { return swapBuffersExecutionTime; }
	void SetSwapBuffersExecutionTime(double time) { swapBuffersExecutionTime = time; }

	// Lights
	void AddLight(LightSourceType type);
	LightSource* GetActiveLight() { return lights[GetActiveLightsIndex()]; }
	const int GetActiveLightsIndex() const { return activeLightsIndex; }
	void SetActiveLightsIndex(const int index) { activeLightsIndex = index; }
	const std::vector<LightSource*>& GetLightsVector() const { return lights; }
	const int GetLightsCount() const { return lights.size(); }
	const glm::vec4& GetAmbientLight() const { return ambientLight; }
	void SetAmbientLight(const glm::vec4& light) { ambientLight = light; }
	bool GetDrawLights() { return showingLights; }
	void SetDrawLights(bool value) { showingLights = value; }
	unsigned int GetLightsNumberLimit() { return MAX_LIGHTS_NUMBER; }

	// Shader
	bool GetUseBumpMapping() { return useBumpMapping; }
	void SetUseBumpMapping(bool value) { useBumpMapping = value; }

	// Fog
	void SetFogEnabled(bool value) { fogEnabled = value; }
	void SetFogStart(float value) { fogStart = value; }
	void SetFogFinish(float value) { fogFinish = value; }
	bool GetFogEnabled() const { return fogEnabled; }
	float GetFogStart() const { return fogStart; }
	float GetFogFinish() const { return fogFinish; }

	// Toon Shading
	void SetToonShading(bool value) { toonShading = value; }
	bool GetToonShading() { return toonShading; }
	void SetToonShadingLevels(int value) { toonShadingLevels = value; }
	int GetToonShadingLevels() { return toonShadingLevels; }

	// Input
	IMoving* GetActiveMovingObject() { return &GetActiveCamera(); }
	IDirectional* GetActiveDirectionalObject() { return &GetActiveCamera(); }
};