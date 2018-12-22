#pragma once
#include<glm/glm.hpp>

struct Point
{
	float X;
	float Y;
	float Z;

	Point() : X(0.0f), Y(0.0f), Z(0.0f) {}
	Point(float x, float y,float z) :X(x), Y(y),Z(z) {}
	Point(const glm::vec4& vector) :X(vector.x),Y(vector.y),Z(vector.z) {}
};