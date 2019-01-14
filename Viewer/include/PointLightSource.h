#pragma once
#include "LightSource.h"

class PointLightSource : public LightSource
{
private:
	static int id;
	glm::vec4 location;
public:
	// Constructors
	PointLightSource() : PointLightSource(glm::vec4(0.0f,0.0f,-1.0f,1.0f)) {}
	PointLightSource(const glm::vec4& _location) : location(_location)
	{
		name = "Point Light Source #" + std::to_string(id++);
	}

	// Base class 
	/*Returns the normalized vector from the worldPoint to the light source*/
	virtual const glm::vec4 GetDirectionToLightSource(const glm::vec4 & worldPoint) const override { return glm::normalize(location - worldPoint); }
	virtual const glm::vec4 * GetDirection() const override { return nullptr; };
	virtual const glm::vec4 * GetLocation() const override { return &location; };

	// Base class setters
	virtual void SetDirection(const glm::vec4 & _direction) override { return; }
	virtual void SetLocation(const glm::vec4 & _location)   override { location = _location; }

	// Inherited via LightSource
	virtual void Move(const glm::vec3 direction) override;
	virtual void RotateX(const float angle) override {} // Point light source emits light everywhere
	virtual void RotateY(const float angle) override {} 
	virtual void RotateZ(const float angle) override {} 

	// Inherited via LightSource
	virtual const glm::mat4x4 & GetWorldTransformation() override;
	virtual const ShadingModels GetShadingMethod() override;
	virtual bool SetShadingMethod(ShadingModels model) override;

	// Inherited via LightSource
	virtual std::vector<glm::vec3>& GetVerticesVector() override;
	virtual std::vector<Face>& GetFacesVector() override;
	virtual const std::vector<glm::vec3>& GetNormalsVector() override;
};

