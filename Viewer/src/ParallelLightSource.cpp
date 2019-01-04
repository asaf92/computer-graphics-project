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