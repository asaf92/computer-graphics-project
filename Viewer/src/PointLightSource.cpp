#include "PointLightSource.h"

PointLightSource::PointLightSource(int id) : 
LightSource(glm::vec4(0.2f), id),
location(glm::vec4(0.0f, 0.0f, -1.0f, 1.0f))
{
	name = "Point Light Source #" + std::to_string(this->GetID());
}

PointLightSource::PointLightSource(const glm::vec4 & location, int id):
LightSource(glm::vec4(0.2f), id),
location(location)
{
	name = "Point Light Source #" + std::to_string(this->GetID());
}