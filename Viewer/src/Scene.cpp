#include "Scene.h"
#include "MeshModel.h"
#include <string>

Scene::Scene() :
	activeCameraIndex(0),
	activeModelIndex(0),
	activeLightsIndex(0),
	ambientLight(glm::vec4(0.25f)),
	selectedShader(Flat)
{
	// Init first camera
	AddCamera(*(new Camera()));
	SetActiveCameraIndex(1);
}

Scene::~Scene()
{
	cameras.clear(); // This calls the destructor on every camera
	lights.clear();
}

void Scene::AddModel(const std::shared_ptr<MeshModel>& model)
{
	models.push_back(model);
}

std::shared_ptr<MeshModel> Scene::GetActiveModel() const
{
	if (models.empty())
	{
		return nullptr;
	}

	int index = GetActiveModelIndex();
	return models[index];
}

const int Scene::GetModelCount() const
{
	return models.size();
}

void Scene::AddCamera(const Camera& camera)
{
	cameras.push_back(camera);
}

const std::vector<Camera>& Scene::GetCamerasVector() const
{
	return cameras;
}

const int Scene::GetCameraCount() const
{
	return cameras.size();
}

void Scene::SetActiveCameraIndex(const int pIndex)
{
	unsigned int index = (unsigned int) pIndex;
	if (index >= 0 && index < cameras.size())
	{
		activeCameraIndex = index;
	}
}

const int Scene::GetActiveCameraIndex() const
{
	return activeCameraIndex;
}

void Scene::AddLight(LightSourceType type)
{
	switch (type)
	{
	case PointSource:
		lights.push_back(new PointLightSource());
		return;
	case Parallel:
		lights.push_back(new ParallelLightSource());
		return;
	default:
		return;
	}
}

void Scene::SetActiveModelIndex(const int pIndex)
{
	unsigned int index = (unsigned int)pIndex;
	if (index >= 0 && index < models.size())
	{
		activeModelIndex = index;
	}
}

const int Scene::GetActiveModelIndex() const
{
	return activeModelIndex;
}

