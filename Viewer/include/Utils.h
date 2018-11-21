#pragma once
#include <glm/glm.hpp>
#include <string>
#include "MeshModel.h"

/*
 * Utils class.
 * This class is consisted of static helper methods that can have many clients across the code.
 */
class Utils
{
public:
	static glm::vec3 Vec3fFromStream(std::istream& issLine);
	static glm::vec2 Vec2fFromStream(std::istream& issLine);
	static glm::vec4 Vec4FromVec3(const glm::vec3 &);
	static glm::vec4 Vec4FromVec3WithZero(const glm::vec3 & other);
	static MeshModel LoadMeshModel(const std::string& filePath);

	// Add here more static utility functions...
	// For example:
	//	1. function that gets an angle, and returns a rotation matrix around a certian axis
	//	2. function that gets a vector, and returns a translation matrix
	//	3. anything else that may be useful in global scope

private:
	static std::string GetFileName(const std::string& filePath);
};
