#pragma once

#include "MeshModel.h"
using namespace std;

class Cube : public IMeshObject, public IShaded
{
private:
	float length;
	float width;
	float height;

	std::vector<glm::vec3> normals;
	vector<Face> faces;

	GLuint vao;
	GLuint vbo;
	std::vector<Vertex> modelVertices;
public:
	Cube(): Cube(0.1f){}
	Cube(float size);
	~Cube();

	// IShaded
	virtual const glm::mat4x4 GetWorldTransformationOld() { return glm::mat4x4(1); }
	virtual const ShadingModels GetShadingMethod() { return Flat; };
	virtual bool SetShadingMethod(ShadingModels model) { return false; };

	// Inherited via IMeshObject
	virtual const GLuint & GetVao() const override { return vao; }
	virtual const unsigned int GetNumberOfVertices() const override { return modelVertices.size(); }
	virtual const glm::mat4 GetWorldTransformation() const override { return glm::mat4(1.0f); }
	virtual const glm::mat4 GetModelTransformation() const override { return glm::mat4(1.0f);}
};