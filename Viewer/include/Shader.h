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
	// Dependencies
	Scene& scene;

	// Properties
	ShadingModels selectedModel;

	// Colors
	const glm::vec4& ambientColor;

	// Calculations
	const glm::vec4 CalculateColorPhong(const glm::vec4& color) const;
	const glm::vec4 calculateAmbientPart(const glm::vec4& color) const;

public:
	Shader(Scene& scene): scene(scene), ambientColor(scene.GetAmbientLight()), selectedModel(Phong) {}
	const glm::vec4 CalculateColor(const glm::vec4& color) const;
};
