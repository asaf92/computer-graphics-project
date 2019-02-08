#pragma once
#include "LightSource.h"
#include "Cube.h"

class PointLightSource : public LightSource, public IMovable
{
private:
	static int id;
	glm::vec4 location;
	Cube cubeModel;
public:
	// Constructors
	PointLightSource() : PointLightSource(glm::vec4(0.5f,0.5f,0.5f,1.0f)) {}
	PointLightSource(const glm::vec4& _location);


	// Base class 
	virtual const glm::vec4 GetLocation() const override { return location; };

	// Inherited via IMovable
	virtual void Move(const glm::vec3 direction) override;

	// Inherited via LightSource
	virtual const GLuint & GetVao()                  const override { return cubeModel.GetVao(); }
	virtual const unsigned int GetNumberOfVertices() const override { return cubeModel.GetNumberOfVertices(); }
	virtual const glm::mat4 GetWorldTransformation() const override { return cubeModel.GetWorldTransformation(); }
	virtual const glm::mat4 GetModelTransformation() const override { return cubeModel.GetModelTransformation(); }
};
