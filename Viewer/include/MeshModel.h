#pragma once
#include <glm/glm.hpp>
#include <string>
#include <memory>
#include "Face.h"

/*
 * MeshModel class.
 * This class represents a mesh model (with faces and normals informations).
 * You can use Utils::LoadMeshModel to create instances of this class from .obj files.
 */
class MeshModel
{
protected:
	std::vector<Face> faces;
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	glm::mat4x4 worldTransform;
	glm::vec4 color;
	std::string modelName;
	glm::vec3 centerPoint;
	glm::vec3 minimums;
	glm::vec3 maximums;

public:
	MeshModel(const std::vector<Face>& faces, const std::vector<glm::vec3>& vertices, const std::vector<glm::vec3>& normals, const std::string& modelName = "");
	virtual ~MeshModel();

	void SetWorldTransformation(const glm::mat4x4& worldTransform);
	const glm::mat4x4& GetWorldTransformation() const;

	const glm::vec4& GetColor() const;
	void SetColor(const glm::vec4& color);

	const std::string& GetModelName();

	// Add more methods/functionality as needed...
	
	//Getters
	std::vector<glm::vec3>& GetVerticesVector() { return vertices; }
	std::vector<Face>& GetFacesVector() { return faces; }
	glm::vec3& GetCenterPoint()	   { return centerPoint; }
	glm::vec3& GetMinimumsVector() { return minimums; }
	glm::vec3& GetMaximumVectors() { return maximums; }
};
