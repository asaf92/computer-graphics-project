#pragma once
#include "glm/glm.hpp"

enum LightSourceType
{
	PointSource,
	Parallel
};

class LightSource
{
protected:
	glm::vec4 color;
	glm::vec4 direction;
public:
	LightSource(const glm::vec4& _direction, const glm::vec4& _color) : direction(_direction), color(_color) {	}
};