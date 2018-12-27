#pragma once
#include "LightSource.h"

class PointLightSource : public LightSource
{
protected:
	glm::vec4 location;
public:
	// Constructors
	PointLightSource(int id) : LightSource(glm::vec4(1.0f), 
		                       glm::vec4(0.0f, 0.0f, 1.0f, 1.0f), id), 
							   location(glm::vec4(0.0f, 0.0f, -1.0f, 1.0f)) {}
	PointLightSource(const glm::vec4& _color, 
		             const glm::vec4& _direction, 
		             const glm::vec4& _location, 
		             int id): 
					 LightSource(_color, _direction, id), 
		             location(_location) {}

	const glm::vec4* GetLocation() override { return &location; }
};