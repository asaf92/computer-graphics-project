#pragma once
#include <glm/glm.hpp>
#include "Scene.h"
#include "Utils.h"
#include "Point.h"
#include "BycentricCoordinates.h"

/* Shader class*/
class Shader
{
protected:
	// Dependencies
	Scene& scene;

	// Points
	glm::vec3 ScreenPointA;	
	glm::vec3 ScreenPointB;	
	glm::vec3 ScreenPointC; 
	glm::vec4 WorldPointA;
	glm::vec4 WorldPointB;
	glm::vec4 WorldPointC;
	glm::vec4 CameraWorldPoint;

	// Coordinates
	int x, y; // x and y are current coordinates, xLeft and xRight are the limits of the scanline
	BycentricCoordinates bycentricCoordinates;

	// Normals
	glm::vec4 NormalA;
	glm::vec4 NormalB;
	glm::vec4 NormalC;

	// Colors
	const glm::vec4& ambientColor;
	glm::vec4 objectColor;
	glm::vec4 objectDiffuseColor;
	glm::vec4 objectSpecularColor;
	glm::vec4 colorA;
	glm::vec4 colorB;
	glm::vec4 colorC;

	// Shading methods
	const glm::vec4 calculateColorFlat  () const;
	const glm::vec4 calculateColorPhong () const;
	const glm::vec4 calculateColorGouraud() const;

	// Phong reflection calculation methods
	const glm::vec4 calculatePhongReflection(const glm::vec4& normal, const glm::vec4& worldPoint, const glm::vec4& toCamera) const;
	const glm::vec4 calculateAmbientPart() const;
	const glm::vec4 calculateDiffusePart(const glm::vec4& normal, const glm::vec4& worldPoint, const LightSource* lightSource) const;
	const glm::vec4 calculateSpectralPart(const glm::vec4& normal, const glm::vec4& worldPoint, const glm::vec4& toCamera, const LightSource* lightSource) const;

	// Vector calculations
	const glm::vec4 calculateFaceWorldCenter() const { return (WorldPointA + WorldPointB + WorldPointC / 3.0f); }
	const glm::vec4 calculateToCameraVector(const glm::vec4& worldPoint) const { return CameraWorldPoint - worldPoint; }

public:
	Shader(Scene& scene): scene(scene), ambientColor(scene.GetAmbientLight()) {}
	
	const glm::vec4 GetColor() const;

	void SetObjectColor(const glm::vec4& color)      { objectColor = color; }
	void SetCameraWorldPoint(const glm::vec4& point)     { CameraWorldPoint = point; }
	void SetNormals(const glm::vec4& a, const glm::vec4& b, const glm::vec4& c);
	void SetWorldPoints (const glm::vec4& worldPointA , const glm::vec4& worldPointB , const glm::vec4& worldPointC);
	void SetScreenPoints(const glm::vec3 & screenPointA, const glm::vec3 & screenPointB, const glm::vec3 & screenPointC);
	void SetCoords(const int _x, const int _y) { x=_x; y=_y; }											   // Not sure these are needed
	void SetBycentricCoords(const BycentricCoordinates coordinates) { bycentricCoordinates = coordinates; }
	void SetObjectDiffuseColor(const glm::vec4& color) { objectDiffuseColor = color; }
	void SetObjectSpecularColor(const glm::vec4& color) { objectSpecularColor = color; }
	void CalculateVertexColors();
};
