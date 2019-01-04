#include "LightSource.h"

void LightSource::Move(const glm::vec3 direction)
{
	auto vec4Direction = Utils::Vec4FromVec3DirectionVector(direction);
	auto currentLocation = GetLocation();
	SetLocation(*currentLocation + vec4Direction);
}
