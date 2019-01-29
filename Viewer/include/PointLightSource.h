#pragma once
#include "LightSource.h"
#include "Cube.h"

class PointLightSource : public LightSource, public IMovable
{
private:
	static int id;
	IMeshObject* model;
	glm::vec4 location;
	Cube cubeModel;
public:
	// Constructors
	PointLightSource() : PointLightSource(glm::vec4(0.0f,0.0f,-1.0f,1.0f)) {}
	PointLightSource(const glm::vec4& _location) : location(_location), model()
	{
		model = &cubeModel;
		name = "Point Light Source #" + std::to_string(id++);
	}

	// Base class 
	virtual const glm::vec4 GetLocation() const override { return location; };

	// Inherited via IMovable
	virtual void Move(const glm::vec3 direction) override;

	// Inherited via LightSource
	virtual const GLuint & GetVao()                  const override { return model->GetVao(); }
	virtual const unsigned int GetNumberOfVertices() const override { return model->GetNumberOfVertices(); }
	virtual const glm::mat4 GetWorldTransformation() const override { return model->GetWorldTransformation(); }
	virtual const glm::mat4 GetModelTransformation() const override { return model->GetModelTransformation(); }
};
