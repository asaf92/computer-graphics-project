#pragma once
#include "LightSource.h"
#include "Globals.h"
#include "Cube.h"

class ParallelLightSource : public LightSource
{
private:
	static int id;
	glm::vec4 direction;
	IMeshObject* model;
	Cube cubeModel;
public:
	// Constructor
	ParallelLightSource();

	// Setters
	virtual void SetDirection(const glm::vec4 & _direction) override { direction = glm::normalize(_direction); }
	virtual void SetLocation(const glm::vec4 & _location) override { return; }

	// Getters
	virtual const glm::vec4 * GetDirection() const override { return &direction; }
	virtual const glm::vec4 * GetLocation() const override { return nullptr; };

	// Methods
	virtual const glm::vec4 GetDirectionToLightSource(const glm::vec4 & worldPoint) const override { return -direction; };

	// Inherited via LightSource
	virtual void RotateX(const float angle) override;
	virtual void RotateY(const float angle) override;
	virtual void RotateZ(const float angle) override;

	// Can't move a parallel light source
	virtual void Move(const glm::vec3 direction) override {}

	// Inherited via LightSource
	virtual const GLuint & GetVao()                  const override {return model->GetVao();}
	virtual const unsigned int GetNumberOfVertices() const override {return model->GetNumberOfVertices();}
	virtual const glm::mat4 GetWorldTransformation() const override {return model->GetWorldTransformation();}
	virtual const glm::mat4 GetModelTransformation() const override {return model->GetModelTransformation();}
};
