#pragma once
#include "IMovable.h"
#include "IRotatable.h"
#include "IMeshObject.h"
#include "glm/glm.hpp"
#include "Utils.h"
#include <string>

enum LightSourceType
{
	PointSource,
	Parallel
};

// Abstract class
class LightSource: public IMeshObject
{
protected:
	glm::vec4 color;
	std::string name;
public:
	// Constructors
	LightSource() :color(glm::vec4(0.3f,0.3f,0.3f,1.0f)) {}
	LightSource(const glm::vec4& color): color(color) {}
	
	// Setters
	void SetColor(const glm::vec4& _color){ color = _color;}

	// Getters
	const glm::vec4& GetColor() const { return color; }
	const std::string GetName() const { return name; }

	// Virtual Getters
	virtual const glm::vec4 GetLocation()  const =0;

	// Inherited via IMeshObject
	virtual const GLuint & GetVao() const override =0;
	virtual const unsigned int GetNumberOfVertices() const override =0;
	virtual const glm::mat4 GetWorldTransformation() const override =0;
	virtual const glm::mat4 GetModelTransformation() const override =0;
};