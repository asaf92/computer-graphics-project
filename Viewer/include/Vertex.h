#pragma once
#include <glm/glm.hpp>

struct Vertex
{
	glm::vec3 position;
	glm::vec3 normal;

	Vertex(glm::vec3 _position, glm::vec3 _normal) : position(_position), normal(_normal) { }
	Vertex(glm::vec3 pos) : Vertex(pos,glm::vec3(0.0f)) {}
	Vertex(): Vertex(glm::vec3(0.0f),glm::vec3(0.0f))   {}
};