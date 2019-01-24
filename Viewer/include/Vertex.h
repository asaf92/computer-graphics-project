#pragma once
#include <glm/glm.hpp>

struct Vertex
{
	glm::vec3 positions;

	Vertex():positions(glm::vec3(0.0f)) {}
	Vertex(glm::vec3 pos): positions(pos) { }
};