#pragma once
#include "LightSource.h"
#include "Globals.h"
#include "Cube.h"

class ParallelLightSource : public LightSource
{
private:
	static int id;
	glm::vec4 direction;
	Cube model;
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
	virtual const glm::mat4x4  GetWorldTransformation() override;
	virtual const ShadingModels GetShadingMethod() override;
	virtual bool SetShadingMethod(ShadingModels model) override;
	virtual std::vector<glm::vec3> GetVerticesVector() override;
	virtual std::vector<Face> GetFacesVector() override;
	virtual const std::vector<glm::vec3> GetNormalsVector() override;
};
