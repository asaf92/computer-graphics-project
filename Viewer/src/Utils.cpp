#include "Utils.h"
#include <cmath>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

glm::vec3 Utils::Vec3fFromStream(std::istream& issLine)
{
	float x, y, z;
	issLine >> x >> std::ws >> y >> std::ws >> z;
	return glm::vec3(x, y, z);
}

glm::vec2 Utils::Vec2fFromStream(std::istream& issLine)
{
	float x, y;
	issLine >> x >> std::ws >> y;
	return glm::vec2(x, y);
}

glm::vec3 Utils::Vec3FromVec4(const glm::vec4& other)
{
	return glm::vec3(other.x, other.y, other.z);
}

glm::vec4 Utils::Vec4FromVec3Point(const glm::vec3& other)
{
	glm::vec4 out;
	out.x = other.x;
	out.y = other.y;
	out.z = other.z;
	out.w = 1;

	return out;
}

glm::vec4 Utils::Vec4FromVec3DirectionVector(const glm::vec3& other)
{
	glm::vec4 out;
	out.x = other.x;
	out.y = other.y;
	out.z = other.z;
	out.w = 0;

	return out;
}

glm::vec4 Utils::Vec4FromPoint(const Point& point)
{
	return glm::vec4(point.X,
					 point.Y,
					 point.Z,
					 1.0f);
}

glm::vec4 Utils::Vec4FromVec3WithZero(const glm::vec3& other)
{
	glm::vec4 out;
	out.x = other.x;
	out.y = other.y;
	out.z = other.z;
	out.w = 0;

	return out;
}

/*Takes the pixel from [-1, 1] to [0,viewportHeight/Width]
Does NOT round the values*/
glm::vec3 Utils::ScreenVec3FromWorldPoint(const Point& _worldPoint,int _viewportWidth, int _viewportHeight)
{
	glm::vec4 worldPoint = Vec4FromPoint(_worldPoint);
	float viewportWidth = float(_viewportWidth);
	float viewportHeight = float(_viewportHeight);
	worldPoint += 1.0f;
	worldPoint /= 2.0f;
	worldPoint.x *= viewportWidth;
	worldPoint.y *= viewportHeight;

	return glm::vec3(worldPoint.x, worldPoint.y, worldPoint.z);
}

glm::vec3 Utils::ScreenVec3FromWorldPoint(const glm::vec4& _worldPoint, int _viewportWidth, int _viewportHeight)
{
	glm::vec3 screenPoint = glm::vec3(_worldPoint.x,_worldPoint.y,_worldPoint.z);
	float viewportWidth = float(_viewportWidth);
	float viewportHeight = float(_viewportHeight);
	screenPoint.x += 1.0f;
	screenPoint.y += 1.0f;
	screenPoint.z += 1.0f;
	screenPoint.x /= 2.0f;
	screenPoint.y /= 2.0f;
	screenPoint.z /= 2.0f;
	screenPoint.x *= viewportWidth;
	screenPoint.y *= viewportHeight;

	return screenPoint;
}

MeshModel Utils::LoadMeshModel(const std::string& filePath)
{
	std::vector<Face> faces;
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec2> textureCoords;
	std::ifstream ifile(filePath.c_str());

	// while not end of file
	while (!ifile.eof())
	{
		// get line
		std::string curLine;
		std::getline(ifile, curLine);

		// read the type of the line
		std::istringstream issLine(curLine);
		std::string lineType;

		issLine >> std::ws >> lineType;

		// based on the type parse data
		if (lineType == "v")
		{
			vertices.push_back(Utils::Vec3fFromStream(issLine));
		}
		else if (lineType == "vn")
		{
			normals.push_back(Utils::Vec3fFromStream(issLine));
		}
		else if (lineType == "vt")
		{
			textureCoords.push_back(Utils::Vec2fFromStream(issLine));
		}
		else if (lineType == "f")
		{
			faces.push_back(Face(issLine));
		}
		else if (lineType == "#" || lineType == "")
		{
			// comment / empty line
		}
		else
		{
			std::cout << "Found unknown line Type \"" << lineType << "\"";
		}
	}

	//return MeshModel(faces, vertices, normals, Utils::GetFileName(filePath));
	return MeshModel(faces, vertices, normals, textureCoords, Utils::GetFileName(filePath));
}

std::string Utils::GetFileName(const std::string& filePath)
{
	if (filePath.empty()) {
		return {};
	}

	auto len = filePath.length();
	auto index = filePath.find_last_of("/\\");

	if (index == std::string::npos) {
		return filePath;
	}

	if (index + 1 >= len) {

		len--;
		index = filePath.substr(0, len).find_last_of("/\\");

		if (len == 0) {
			return filePath;
		}

		if (index == 0) {
			return filePath.substr(1, len - 1);
		}

		if (index == std::string::npos) {
			return filePath.substr(0, len);
		}

		return filePath.substr(index + 1, len - index - 1);
	}

	return filePath.substr(index + 1, len - index);
}

std::vector<glm::vec3> Utils::CalculateNormals(std::vector<glm::vec3> vertices, std::vector<Face> faces)
{
	std::vector<glm::vec3> normals(vertices.size());
	std::vector<int> adjacent_faces_count(vertices.size());

	for (int i = 0; i < adjacent_faces_count.size(); i++)
	{
		adjacent_faces_count[i] = 0;
	}

	for (int i = 0; i < faces.size(); i++)
	{
		Face currentFace = faces.at(i);

		int index0 = currentFace.GetVertexIndex(0) - 1;
		int index1 = currentFace.GetVertexIndex(1) - 1;
		int index2 = currentFace.GetVertexIndex(2) - 1;

		glm::vec3 v0 = vertices.at(index0);
		glm::vec3 v1 = vertices.at(index1);
		glm::vec3 v2 = vertices.at(index2);

		glm::vec3 u = v0 - v1;
		glm::vec3 v = v2 - v1;
		glm::vec3 face_normal = glm::normalize(-glm::cross(u, v));

		normals.at(index0) += face_normal;
		normals.at(index1) += face_normal;
		normals.at(index2) += face_normal;

		adjacent_faces_count.at(index0) += 1;
		adjacent_faces_count.at(index1) += 1;
		adjacent_faces_count.at(index2) += 1;
	}

	for (int i = 0; i < normals.size(); i++)
	{
		normals[i] /= adjacent_faces_count[i];
		normals[i] = glm::normalize(normals[i]);
	}

	return normals;
}