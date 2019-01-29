#include "ParallelLightSource.h"

int ParallelLightSource::id = 0;

ParallelLightSource::ParallelLightSource(): LightSource(), cubeModel(-direction * PARALLEL_DRAWING_DISTANCE, 0.3f)
{
	name = "Parallel Light #" + std::to_string(id++);
	direction = glm::vec4(0.0f, -1.0f, 0.0f, 0.0f);
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

#pragma region IMeshObject

#pragma endregion