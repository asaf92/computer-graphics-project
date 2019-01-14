#pragma once

#include "MeshModel.h"

class Cube : public IMeshObject, public IShaded
{
private:
	float length;
	float width;
	float height;
public:
	Cube() : length(1.0f), width(1.0f), height(1.0f) {}

	// IShaded
	virtual const glm::mat4x4& GetWorldTransformation() { return glm::mat4x4(1); }
	virtual const ShadingModels GetShadingMethod() { return Flat; };
	virtual bool SetShadingMethod(ShadingModels model) { return false; };

	// IMeshObject
	virtual std::vector<glm::vec3>& GetVerticesVector();
	virtual std::vector<Face>& GetFacesVector();
	virtual const std::vector<glm::vec3>& GetNormalsVector() ;
};