#pragma once
#include<glm/glm.hpp>

class Material
{
private:
	glm::vec4 ambientColor;
	glm::vec4 diffuseColor;
	glm::vec4 specularColor;
public:
	// Constructors
	Material() : ambientColor(glm::vec4(0.0f)), diffuseColor(glm::vec4(0.25f, 0.25f, 0.25f, 1.0f)), specularColor(glm::vec4(0.0f)) {}
	Material(const Material& other) : ambientColor(other.GetAmbientColor()), diffuseColor(other.GetDiffuseColor()), specularColor(other.GetSpecularColor()) {}

	// Setters
	void SetAmbientColor(const glm::vec4& color) { ambientColor = color; }
	void SetDiffuseColor(const glm::vec4& color) { diffuseColor = color; }
	void SetSpecularColor(const glm::vec4& color) { specularColor = color; }

	// Getters
	glm::vec4 GetAmbientColor()  const { return ambientColor; }
	glm::vec4 GetDiffuseColor()  const { return diffuseColor; }
	glm::vec4 GetSpecularColor() const { return specularColor;}
};