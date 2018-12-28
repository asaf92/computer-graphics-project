#pragma once
#include "glm/glm.hpp"
#include <string>

enum LightSourceType
{
	PointSource,
	Parallel
};

// Abstract class
class LightSource
{
protected:
	glm::vec4 color;
	std::string name;
	int ID;
public:
	// Constructors
	LightSource(const glm::vec4& color,int id): color(color), ID(id) {}
	
	// Setters
	void SetColor(const glm::vec4& _color){ color = _color;}

	// Getters
	const glm::vec4& GetColor() const { return color; }
	const int GetID()           const { return ID;    }
	const std::string GetName() const { return name; }

	// Virtual Methods
	virtual const glm::vec4 GetDirectionToLightSource(const glm::vec4& worldPoint) const = 0;

	// Virtual Setters
	virtual void SetDirection(const glm::vec4& _direction) =0;
	virtual void SetLocation (const glm::vec4& _location)  =0;

	// Virtual Getters
	virtual const glm::vec4* GetDirection() const =0;
	virtual const glm::vec4* GetLocation()  const =0;
};