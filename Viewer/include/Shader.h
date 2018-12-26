#pragma once
#include <glm/glm.hpp>

enum ShadingModels {
	Phong,
	Gouraud
};

class Shader
{
private:
	ShadingModels selectedModel;
public:
	glm::vec4& ambientColor;
	Shader(): ambientColor(glm::vec4(0)), selectedModel(Phong) {}
};
