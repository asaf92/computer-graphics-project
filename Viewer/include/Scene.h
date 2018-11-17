#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>
#include <string>
#include <memory>
#include "MeshModel.h"
#include "Camera.h"

/*
 * Scene class.
 * This class holds all the scene information (models, cameras, lights, etc..)
 */
class Scene {
private:
	std::vector<std::shared_ptr<MeshModel>> models;
	std::vector<Camera> cameras;

	int activeCameraIndex;
	int activeModelIndex;

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

	void SetActiveCameraIndex(int index);
	const int GetActiveCameraIndex() const;

	void SetActiveModelIndex(int index);
	const int GetActiveModelIndex() const;

	// Add more methods as needed...
};