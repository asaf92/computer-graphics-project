#pragma once
#include <glm/glm.hpp>

/*
* This interface is implemented by any object that keeps uniform material.
*/
class IUniformMaterial
{
public:
	// Getters
	virtual const glm::vec4 GetAmbientColor()  const = 0;
	virtual const glm::vec4 GetDiffuseColor()  const = 0;
	virtual const glm::vec4 GetSpecularColor() const = 0;
	virtual const float GetShininess()     const = 0;

	// Setters
	virtual void SetAmbientColor (const glm::vec4& color) = 0;
	virtual void SetDiffuseColor (const glm::vec4& color) = 0;
	virtual void SetSpecularColor(const glm::vec4& color) = 0;
	virtual void SetShininess    (const float shininess)  = 0;
};