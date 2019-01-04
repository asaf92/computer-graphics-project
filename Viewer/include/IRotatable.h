#pragma once
#include<glm/glm.hpp>

class IRotatable
{
public:
	virtual ~IRotatable() {}
	virtual void RotateX(const float angle) =0;
	virtual void RotateY(const float angle) =0;
	virtual void RotateZ(const float angle) =0;
};