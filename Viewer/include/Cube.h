#pragma once

#include "MeshModel.h"
using namespace std;

class Cube : public IMeshObject, public IShaded
{
private:
	float length;
	float width;
	float height;

	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	vector<Face> faces;
public:
	Cube(): Cube(0.1f){}
	Cube(float size);

	// IShaded
	virtual const glm::mat4x4 GetWorldTransformation() { return glm::mat4x4(1); }
	virtual const ShadingModels GetShadingMethod() { return Flat; };
	virtual bool SetShadingMethod(ShadingModels model) { return false; };

	// IMeshObject
	virtual std::vector<glm::vec3> GetVerticesVector();
	virtual std::vector<Face> GetFacesVector();
	virtual const std::vector<glm::vec3> GetNormalsVector() ;
};