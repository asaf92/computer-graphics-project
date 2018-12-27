#pragma once
#include "glm/glm.hpp"

enum LightSourceType
{
	PointSource,
	Parallel
};

class LightSource
{
protected:
	glm::vec4 color;
	glm::vec4 direction;
	int ID;
public:
	LightSource(const glm::vec4& _direction, const glm::vec4& _color, int id) : direction(_direction), color(_color), ID(id) {	}
	
	// Setters
	void SetDirection(const glm::vec4& _direction) { direction = _direction; }
	void SetColor(const glm::vec4& _color)         { color = _color; }

	// Getters
	const glm::vec4& GetDirection() const { return direction; }
	const glm::vec4& GetColor()     const { return color; }

	// Virtual Getters
	virtual const glm::vec4* GetLocation() { return nullptr; }
};