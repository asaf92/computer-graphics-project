#include "ParallelLightSource.h"

int ParallelLightSource::id = 0;

ParallelLightSource::ParallelLightSource(): LightSource()
{
	name = "Parallel Light #" + std::to_string(id++);
	direction = glm::vec4(0.0f, -1.0f, 0.0f, 0.0f);
	model = Cube(0.3f);
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
const glm::mat4x4  ParallelLightSource::GetWorldTransformation()
{
	auto worldTransform = glm::mat4x4(1);
	worldTransform[3].x = -direction.x * PARALLEL_DRAWING_DISTANCE;
	worldTransform[3].y = -direction.y * PARALLEL_DRAWING_DISTANCE;
	worldTransform[3].z = -direction.z * PARALLEL_DRAWING_DISTANCE;

	return worldTransform;
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
	return model.GetVerticesVector();
}

std::vector<Face> ParallelLightSource::GetFacesVector()
{
	return model.GetFacesVector();
}

const std::vector<glm::vec3> ParallelLightSource::GetNormalsVector()
{
	return model.GetNormalsVector();
}
#pragma endregion