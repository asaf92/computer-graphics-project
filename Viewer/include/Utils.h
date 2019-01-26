#pragma once
#include <glm/glm.hpp>
#include <string>
#include "MeshModel.h"
#include "Point.h"

/*
 * Utils class.
 * This class is consisted of static helper methods that can have many clients across the code.
 */
class Utils
{
public:
	static glm::vec3 Vec3fFromStream(std::istream& issLine);
	static glm::vec2 Vec2fFromStream(std::istream& issLine);
	static glm::vec3 Vec3FromVec4(const glm::vec4 & other);
	static glm::vec4 Vec4FromVec3Point(const glm::vec3 &);
	static glm::vec4 Vec4FromVec3DirectionVector(const glm::vec3 & other);
	static glm::vec4 Vec4FromPoint(const Point& point);
	static glm::vec4 Vec4FromVec3WithZero(const glm::vec3 & other);
	static glm::vec3 ScreenVec3FromWorldPoint(const Point & _worldPoint, int _viewportWidth, int _viewportHeightPar);
	static glm::vec3 ScreenVec3FromWorldPoint(const glm::vec4 & worldPoint, int _viewportWidth, int _viewportHeight);
	static MeshModel* LoadMeshModel(const std::string& filePath);
	static std::vector<glm::vec3> CalculateNormals(std::vector<glm::vec3> vertices, std::vector<Face> faces);

private:
	static std::string GetFileName(const std::string& filePath);
};
