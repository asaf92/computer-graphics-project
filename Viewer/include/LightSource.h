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
	glm::vec4 location;
	int ID;
public:
	LightSource(const glm::vec4& _direction, const glm::vec4& _color, int id, const glm::vec4& _location) : direction(_direction), color(_color), ID(id), location(_location) {	}
	
	// Setters
	void SetDirection(const glm::vec4& _direction) { direction = _direction; }
	void SetColor(const glm::vec4& _color)         { color     = _color;     }
	void SetLocation(const glm::vec4& _location)   { location  = _location;  }
	 
	// Getters
	const glm::vec4& GetDirection() const { return direction;}
	const glm::vec4& GetColor()     const { return color;    }
	const int GetID()               const { return ID;       }
	const glm::vec4& GetLocation() const  { return location; }
};