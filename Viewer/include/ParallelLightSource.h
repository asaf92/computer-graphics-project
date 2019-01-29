#pragma once
#include "LightSource.h"
#include "Globals.h"
#include "Cube.h"

constexpr float PARALLEL_LIGHT_POSITION_DISTANCE = 1000.0f;

class ParallelLightSource : public LightSource, public IRotatable
{
private:
	static int id;
	glm::vec4 direction;
	IMeshObject* model;
	Cube cubeModel;
public:
	// Constructor
	ParallelLightSource();

	// Getters
	virtual const glm::vec4 GetLocation() const override { return -direction * PARALLEL_LIGHT_POSITION_DISTANCE; };

	// Inherited via IRotatable
	virtual void RotateX(const float angle) override;
	virtual void RotateY(const float angle) override;
	virtual void RotateZ(const float angle) override;

	// Inherited via LightSource
	virtual const GLuint & GetVao()                  const override {return model->GetVao();}
	virtual const unsigned int GetNumberOfVertices() const override {return model->GetNumberOfVertices();}
	virtual const glm::mat4 GetWorldTransformation() const override {return model->GetWorldTransformation();}
	virtual const glm::mat4 GetModelTransformation() const override {return model->GetModelTransformation();}
};
