#pragma once
#include "IMovable.h"
#include "IRotatable.h"
#include "IShaded.h"
#include "IMeshObject.h"
#include "glm/glm.hpp"
#include "Utils.h"
#include <string>

enum LightSourceType
{
	PointSource,
	Parallel
};

// Abstract class
class LightSource: public IMovable,public IRotatable,public IShaded, public IMeshObject
{
protected:
	glm::vec4 color;
	std::string name;
public:
	// Constructors
	LightSource() :color(glm::vec4(0.3f)) {}
	LightSource(const glm::vec4& color): color(color) {}
	
	// Setters
	void SetColor(const glm::vec4& _color){ color = _color;}

	// Getters
	const glm::vec4& GetColor() const { return color; }
	const std::string GetName() const { return name; }

	// Virtual Methods
	virtual const glm::vec4 GetDirectionToLightSource(const glm::vec4& worldPoint) const = 0;

	// Virtual Setters
	virtual void SetDirection(const glm::vec4& _direction) =0;
	virtual void SetLocation (const glm::vec4& _location)  =0;

	// Virtual Getters
	virtual const glm::vec4* GetDirection() const =0;
	virtual const glm::vec4* GetLocation()  const =0;
	
	// Inherited via IMovable
	virtual void Move(const glm::vec3 direction) override =0;

	// Inherited via IRotatable
	virtual void RotateX(const float angle) override =0;
	virtual void RotateY(const float angle) override =0;
	virtual void RotateZ(const float angle) override =0;

	// Inherited via IShaded
	virtual const glm::mat4x4 & GetWorldTransformation() override =0;
	virtual const ShadingModels GetShadingMethod()		 override =0;
	virtual bool SetShadingMethod(ShadingModels model)   override =0;

	// Inherited via IMeshObject
	virtual std::vector<glm::vec3>&		  GetVerticesVector() = 0;
	virtual std::vector<Face>&			  GetFacesVector()    = 0;
	virtual const std::vector<glm::vec3>& GetNormalsVector()  = 0;
};