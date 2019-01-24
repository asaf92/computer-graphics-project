#include "PointLightSource.h"

int PointLightSource::id = 0;

void PointLightSource::Move(const glm::vec3 direction)
{
	location += Utils::Vec4FromVec3DirectionVector(direction);
}

//const glm::mat4x4  PointLightSource::GetWorldTransformationOld()
//{
//	auto worldTransform = glm::mat4x4(1);
//	worldTransform[3].x = location.x;
//	worldTransform[3].y = location.y;
//	worldTransform[3].z = location.z;
//
//	return worldTransform;
//}

const ShadingModels PointLightSource::GetShadingMethod()
{
	return Flat;
}

bool PointLightSource::SetShadingMethod(ShadingModels model)
{
	// shading method on point light source is always flat
	return false;
}

const glm::mat4x4 PointLightSource::GetWorldTransformationOld()
{
	return glm::mat4x4();
}
