#include "ParallelLightSource.h"

int ParallelLightSource::id = 0;

#pragma region IRotatable
void ParallelLightSource::RotateX(const float angle)
{
	direction.x += angle;
}
void ParallelLightSource::RotateY(const float angle)
{
	direction.y += angle;
}
void ParallelLightSource::RotateZ(const float angle)
{
	direction.z += angle;
}
#pragma endregion

#pragma region IShaded
const glm::mat4x4  ParallelLightSource::GetWorldTransformation()
{
	return glm::mat4x4(0);
}

const ShadingModels ParallelLightSource::GetShadingMethod()
{
	return Flat;
}

bool ParallelLightSource::SetShadingMethod(ShadingModels model)
{
	return false;
}
#pragma endregion

#pragma region IMeshObject
std::vector<glm::vec3> ParallelLightSource::GetVerticesVector()
{
	return std::vector<glm::vec3>();
}

std::vector<Face> ParallelLightSource::GetFacesVector()
{
	return std::vector<Face>();
}

const std::vector<glm::vec3> ParallelLightSource::GetNormalsVector()
{
	return std::vector<glm::vec3>();
}
#pragma endregion