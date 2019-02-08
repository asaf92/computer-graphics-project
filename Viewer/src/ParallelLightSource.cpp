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
	normalizeDirection();
}
void ParallelLightSource::RotateY(const float angle)
{
	direction.y += angle;
	normalizeDirection();
}
void ParallelLightSource::RotateZ(const float angle)
{
	direction.z += angle;
	normalizeDirection();
}
#pragma endregion

#pragma region IDirectional

glm::vec3 ParallelLightSource::GetDirection()
{
	return direction;
}

void ParallelLightSource::SetDirection(const glm::vec3 & _direction)
{
	direction = Utils::Vec4FromVec3DirectionVector(_direction);
	normalizeDirection();
}

void ParallelLightSource::Tilt(const float angle)
{
	direction = Utils::rotationMatrix(glm::vec3(angle, 0.0f, 0.0f)) * direction;
}

void ParallelLightSource::Pan(const float angle)
{
	direction = Utils::rotationMatrix(glm::vec3(0.0f, angle, 0.0f)) * direction;
}

#pragma endregion

void ParallelLightSource::normalizeDirection()
{
	if (direction != glm::vec4(0.0f))
	{
		this->direction = Utils::Vec4FromVec3DirectionVector(glm::normalize(direction));
	}
}