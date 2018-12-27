#pragma once
#include <glm/glm.hpp>
#include "Scene.h"
#include "Utils.h"
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

	// Shader properties
	ShadingModels selectedModel;

	// Points
	Point ScreenPointA;	// Not sure shader needs to know these
	Point ScreenPointB;	// Not sure shader needs to know these
	Point ScreenPointC; // Not sure shader needs to know these
	glm::vec4 WorldPointA;
	glm::vec4 WorldPointB;
	glm::vec4 WorldPointC;
	glm::vec4 CameraWorldPoint;

	// Coordinates
	int x, y;

	// Normals
	glm::vec4 NormalA;
	glm::vec4 NormalB;
	glm::vec4 NormalC;

	// Colors
	const glm::vec4& ambientColor;
	glm::vec4 objectColor;
	glm::vec4 objectDiffuseColor;

	// Calculation methods
	const glm::vec4 calculateColorFlat  () const;
	const glm::vec4 calculateColorPhong () const;
	const glm::vec4 calculatePhongReflection(const glm::vec4& normal, const glm::vec4& worldPoint, const glm::vec4& toCamera) const;
	const glm::vec4 calculateAmbientPart() const;
	const glm::vec4 calculateDiffusePart(const glm::vec4& normal, const glm::vec4& worldPoint, const LightSource* lightSource) const;
	const glm::vec4 calculateSpectralPart() const {return glm::vec4(0);} // Implement later
	const glm::vec4 calculateFaceWorldCenter() const { return (WorldPointA + WorldPointB + WorldPointC / 3.0f); }
	const glm::vec4 calculateToCameraVector(const glm::vec4& worldPoint) const { return CameraWorldPoint - worldPoint; }

public:
	Shader(Scene& scene): scene(scene), ambientColor(scene.GetAmbientLight()), selectedModel(Flat) {}
	
	const glm::vec4 GetColor() const;
	void SetObjectColor(const glm::vec4& color)      { objectColor = color; }
	void SetShadingModel(const ShadingModels& model) { selectedModel = model; }
	void SetCameraWorldPoint(const glm::vec4& point)     { CameraWorldPoint = Utils::Vec4FromPoint(point); }
	void SetNormals(const glm::vec4& a, const glm::vec4& b, const glm::vec4& c) { NormalA = a; NormalB = b; NormalC = c;}
	void SetWorldPoints (const Point& worldPointA , const Point& worldPointB , const Point& worldPointC);
	void SetScreenPoints(const Point& ScreenPointA, const Point& ScreenPointB, const Point& ScreenPointC);
	void SetCoords(const int _x, const int _y) { x=_x; y=_y; }
	void SetObjectDiffuseColor(const glm::vec4& color) { objectDiffuseColor = color; }
};
