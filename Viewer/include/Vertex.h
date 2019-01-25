#pragma once
#include <glm/glm.hpp>

struct Vertex
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 textureCoords;

	Vertex(glm::vec3 _position, glm::vec3 _normal, glm::vec2 _textureCoords) : position(_position), normal(_normal), textureCoords(_textureCoords) { }
	Vertex(glm::vec3 pos, glm::vec3 _normal) : Vertex(pos, _normal, glm::vec2(0.0f)) {}
	Vertex(glm::vec3 pos) : Vertex(pos,glm::vec3(0.0f),glm::vec2(0.0f)) {}
	Vertex() : Vertex(glm::vec3(0.0f), glm::vec3(0.0f), glm::vec2(0.0f)) {}
};