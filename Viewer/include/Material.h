#pragma once
#include<glm/glm.hpp>

class Material
{
private:
	glm::vec4 ambientColor;
	glm::vec4 diffuseColor;
	glm::vec4 specularColor;
	float shininess;
public:
	// Constructors
	Material() : ambientColor(glm::vec4(0.1f)), diffuseColor(glm::vec4(0.35f, 0.35f, 0.35f, 1.0f)), specularColor(glm::vec4(1.0f,1.0f,1.0f,1.0)) { shininess = 40.0f; }
	Material(const Material& other) : ambientColor(other.GetAmbientColor()), diffuseColor(other.GetDiffuseColor()), specularColor(other.GetSpecularColor()) { shininess = other.GetShininess(); }

	// Setters
	void SetAmbientColor(const glm::vec4& color) { ambientColor = color; }
	void SetDiffuseColor(const glm::vec4& color) { diffuseColor = color; }
	void SetSpecularColor(const glm::vec4& color) { specularColor = color; }
	void SetShininess(const float _shininess) { shininess = _shininess; }

	// Getters
	glm::vec4 GetAmbientColor()  const { return ambientColor; }
	glm::vec4 GetDiffuseColor()  const { return diffuseColor; }
	glm::vec4 GetSpecularColor() const { return specularColor;}
	float GetShininess() const { return shininess; }
};