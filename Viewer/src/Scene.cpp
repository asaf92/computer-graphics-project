#include "Scene.h"
#include "MeshModel.h"
#include <string>

Scene::Scene() :
	activeCameraIndex(0),
	activeModelIndex(0),
	activeLightsIndex(0),
	selectedShader(Phong),
	fogStart(1.0f),
	fogFinish(5.0f),
	worldRadius(10.0f),
	showingLights(true),
	clearColor(0.2f, 0.2f, 0.2f, 1.0f),
	ambientLight(0.2f,0.2f,0.2f,1.0f),
	floor(glm::vec4(0.0f,0.0f,0.0f,1.0f),worldRadius,worldRadius,0.01f)
{
	// Init first camera
	AddCamera(*(new Camera()));
	SetActiveCameraIndex(1);

	// Add default light
	AddLight(PointSource);
	floor.SetAmbientColor(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));
	floor.SetShininess(50.0f);
}

Scene::~Scene()
{
	cameras.clear(); // This calls the destructor on every camera
	lights.clear();
	models.clear();
}

void Scene::AddModel(MeshModel* const model)
{
	models.push_back(model);
}

MeshModel* Scene::GetActiveModel() const
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

