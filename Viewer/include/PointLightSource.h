#pragma once
#include "LightSource.h"

class PointLightSource : public LightSource
{
protected:
	glm::vec4 location;
public:
	// Constructors
	PointLightSource(int id);
	PointLightSource(const glm::vec4& location, int id);

	// Base class 
	virtual const glm::vec4 GetDirectionToLightSource(const glm::vec4 & worldPoint) const override { return location - worldPoint; }
	virtual const glm::vec4 * GetDirection() const override { return nullptr; };
	virtual const glm::vec4 * GetLocation() const override { return &location; };

	// Base class setters
	virtual void SetDirection(const glm::vec4 & _direction) override { return; }
	virtual void SetLocation(const glm::vec4 & _location)   override { location = _location; }
};