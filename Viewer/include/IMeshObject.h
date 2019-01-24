#pragma once
#include <glm/glm.hpp>
#include <glad/glad.h>
#include <vector>
#include "ShadingModels.h"
#include "Face.h"
#include "Vertex.h"

/*
* Interface for all the objects that are presentable
* 
*/
class IMeshObject
{
public:
	
	// Mesh data
	virtual const GLuint&      GetVao() const = 0;
	virtual const unsigned int GetNumberOfVertices() const = 0;
	virtual const glm::mat4    GetWorldTransformation() const = 0;
	virtual const glm::mat4    GetModelTransformation() const = 0;
};