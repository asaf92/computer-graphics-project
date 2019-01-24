#include "ParallelLightSource.h"

int ParallelLightSource::id = 0;

ParallelLightSource::ParallelLightSource(): LightSource()
{
	name = "Parallel Light #" + std::to_string(id++);
	direction = glm::vec4(0.0f, -1.0f, 0.0f, 0.0f);
	cubeModel = Cube(0.3f);
	model = &cubeModel;
}

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

const ShadingModels ParallelLightSource::GetShadingMethod()
{
	return Flat;
}

bool ParallelLightSource::SetShadingMethod(ShadingModels model)
{
	return false;
}
const glm::mat4x4 ParallelLightSource::GetWorldTransformationOld()
{
	return glm::mat4x4();
}
#pragma endregion

#pragma region IMeshObject

#pragma endregion