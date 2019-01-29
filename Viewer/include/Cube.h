#pragma once
#include "MeshModel.h"
#include "Utils.h"
#include <glm/glm.hpp>

using namespace std;

class Cube : public MeshModel
{
private:
	float length;
	float width;
	float height;
	glm::vec4 location;

	std::vector<glm::vec3> normals;
	vector<Face> faces;

public:
	Cube(glm::vec4 location): Cube(location, 0.005f){}
	Cube(glm::vec4 location,float size);
	Cube(glm::vec4 location, float length, float width, float height);
	~Cube();

	virtual const GLuint & GetVao() const override { return vao; }
	virtual const unsigned int GetNumberOfVertices() const override { return modelVertices.size(); }
	virtual const glm::mat4 GetWorldTransformation() const override { return Utils::TranslationMatrix(location); }
	virtual const glm::mat4 GetModelTransformation() const override { return glm::mat4(1.0f);}

	// Inherited via IMovable
	virtual void Move(const glm::vec3 direction) override { location += Utils::Vec4FromVec3DirectionVector(direction); };
};