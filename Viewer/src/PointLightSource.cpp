#include "PointLightSource.h"

int PointLightSource::id = 0;

void PointLightSource::Move(const glm::vec3 direction)
{
	location += Utils::Vec4FromVec3DirectionVector(direction);
}
