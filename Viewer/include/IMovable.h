#pragma once
#include <glm/glm.hpp>

class IMovable
{
public:
	virtual ~IMovable() {}

	// Move the object in the direction 
	virtual void Move(const glm::vec3 direction) = 0;
};