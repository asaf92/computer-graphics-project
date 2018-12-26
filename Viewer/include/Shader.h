#pragma once
#include <glm/glm.hpp>
#include "Scene.h"

enum ShadingModels {
	Phong,
	Gouraud
};

class Shader
{
private:
	Scene& scene;
	ShadingModels selectedModel;
	glm::vec4& ambientColor;
public:
	Shader(Scene& scene): scene(scene), ambientColor(glm::vec4(0)), selectedModel(Phong) {}

};
