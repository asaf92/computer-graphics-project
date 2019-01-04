#pragma once
#include "LightSource.h"

class ParallelLightSource : public LightSource
{
private:
	static int id;
	glm::vec4 direction;
public:
	// Constructor
	ParallelLightSource()
	{
		name = "Parallel Light #" + std::to_string(id++);
		direction = glm::vec4(0.0f,0.0f,1.0f,0.0f);
	}

	// Setters
	virtual void SetDirection(const glm::vec4 & _direction) override { direction = _direction; }
	virtual void SetLocation(const glm::vec4 & _location) override { return; }

	// Getters
	virtual const glm::vec4 * GetDirection() const override { return &direction; }
	virtual const glm::vec4 * GetLocation() const override { return nullptr; };

	// Methods
	virtual const glm::vec4 GetDirectionToLightSource(const glm::vec4 & worldPoint) const override { return -direction; };
};
