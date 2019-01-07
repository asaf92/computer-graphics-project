#pragma once
#include <glm/glm.hpp>
#include <vector>
#include "ShadingModels.h"
#include "Face.h"

/*
* Interface for all the objects that are presentable
* 
*/
class IMeshObject
{
public:
	// Mesh data
	virtual std::vector<glm::vec3>& GetVerticesVector() =0;
	virtual std::vector<Face>& GetFacesVector() =0;
	virtual const std::vector<glm::vec3>& GetNormalsVector() =0;
};