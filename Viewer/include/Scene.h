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

/*
 * Scene class.
 * This class holds all the scene information (models, cameras, lights, etc..)
 */
class Scene {
private:
	std::vector<std::shared_ptr<MeshModel>> models;
	std::vector<Camera> cameras;
	glm::vec4 ambientLight;

	int activeCameraIndex;
	int activeModelIndex;
	double imGuiRenderExecutionTime;
	double zBufferExecutionTime;
	double colorBufferExecutionTime;
	double swapBuffersExecutionTime;
	double renderExecutionTime;

	bool showNormals = false;
	bool fillTriangles = true;
	bool drawAxis = true;

public:
	Scene();
	~Scene();

	void AddModel(const std::shared_ptr<MeshModel>& model);
	const std::vector<std::shared_ptr<MeshModel>> GetModelsVector() const { return models; };
	std::shared_ptr<MeshModel> GetActiveModel() const;
	const int GetModelCount() const;

	void AddCamera(const Camera& camera);
	const std::vector<Camera>& GetCamerasVector() const;
	const int GetCameraCount() const;

	void SetActiveCameraIndex(const int index);
	const int GetActiveCameraIndex() const;
	Camera& GetActiveCamera() { return cameras[GetActiveCameraIndex()]; }

	void SetActiveModelIndex(const int index);
	const int GetActiveModelIndex() const;

	void SetShowNormals(const bool value) { showNormals = value; }
	bool GetShowNormals() const { return showNormals; }

	void SetFillTriangles(const bool value) { fillTriangles = value; }
	bool GetFillTriangles() const { return fillTriangles; }

	void SetDrawAxis(const bool value) { drawAxis = value; }
	bool GetDrawAxis() const {    return drawAxis; }

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

	const glm::vec4& GetAmbientLight() const { return ambientLight; }
	void SetAmbientLight(const glm::vec4& light) { ambientLight = light; }
};