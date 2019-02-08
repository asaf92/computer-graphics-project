#include "PointLightSource.h"

int PointLightSource::id = 0;

PointLightSource::PointLightSource(const glm::vec4 & _location) : LightSource(glm::vec4(glm::vec3(0.4f), 1.0f)), location(_location), cubeModel(location)
{
	name = "Point Light Source #" + std::to_string(id++);
}

void PointLightSource::Move(const glm::vec3 direction)
{
	location += Utils::Vec4FromVec3DirectionVector(direction);
	cubeModel.Move(direction);
}