#pragma once
#include <glm/glm.hpp>
#include "Scene.h"
#include "Point.h"

enum ShadingModels {
	Phong,
	Gouraud,
	Flat
};

class Shader
{
protected:
	// Dependencies
	Scene& scene;

	// Properties
	ShadingModels selectedModel;

	// Points
	Point ScreenPointA;
	Point ScreenPointB;
	Point ScreenPointC;
	Point WorldPointA;
	Point WorldPointB;
	Point WorldPointC;

	// Coordinates
	int x, y;

	// Normals
	glm::vec4 NormalA;
	glm::vec4 NormalB;
	glm::vec4 NormalC;

	// Colors
	const glm::vec4& ambientColor;
	glm::vec4 objectColor;

	// Calculations
	const glm::vec4 CalculateColorPhong () const;
	const glm::vec4 CalculateColorFlat  () const;
	const glm::vec4 calculateAmbientPart() const;

public:
	Shader(Scene& scene): scene(scene), ambientColor(scene.GetAmbientLight()), selectedModel(Phong) {}
	
	const glm::vec4 GetColor() const;
	void SetObjectColor(const glm::vec4& color)      { objectColor = color; }
	void SetShadingModel(const ShadingModels& model) { selectedModel = model; }
	void SetWorldPoints (const Point& worldPointA , const Point& worldPointB , const Point& worldPointC);
	void SetScreenPoints(const Point& ScreenPointA, const Point& ScreenPointB, const Point& ScreenPointC);
	void SetCoords(const int _x, const int _y) { x=_x; y=_y; }
	void SetNormals(const glm::vec4& a, const glm::vec4& b, const glm::vec4& c) { NormalA = a; NormalB = b; NormalC = c;}
};
