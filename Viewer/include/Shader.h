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
	glm::vec4 objectColor;

	// Calculations
	const glm::vec4 CalculateColorPhong() const;
	const glm::vec4 calculateAmbientPart() const;

public:
	Shader(Scene& scene): scene(scene), ambientColor(scene.GetAmbientLight()), selectedModel(Phong) {}
	
	const glm::vec4 CalculateColor() const;

	void SetObjectColor(const glm::vec4& color) { objectColor = color; }
};
