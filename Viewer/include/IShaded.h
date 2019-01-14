#pragma once
#include<glm/glm.hpp>
#include "ShadingModels.h"

/*
* Everything that gets passed to the shader has to implement this interface
*/
class IShaded
{
public:
	virtual const glm::mat4x4& GetWorldTransformation() = 0;
	virtual const ShadingModels GetShadingMethod() = 0;
	virtual bool SetShadingMethod(ShadingModels model) = 0;
};