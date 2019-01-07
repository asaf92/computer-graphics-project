#pragma once
#include<glm/glm.hpp>

class IScalable
{
public:
	virtual void Scale(const glm::vec3& scaleSize) = 0;
	virtual void Scale(const float scaleSize)      = 0;
	virtual const glm::vec3& GetScale() const      = 0;
};