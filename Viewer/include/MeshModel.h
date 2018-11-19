#pragma once
#include <glm/glm.hpp>
#include <string>
#include <memory>
#include "Face.h"

/*
 * MeshModel class.
 * This class represents a mesh model (with faces and normals informations).
 * You can use Utils::LoadMeshModel to create instances of this class from .obj files.
 *
 * The way I implement this class is that the renderer will feed the move, rotate & scale inputs to 
 * the GetWorldTransform function and it will return a 4x4 transformation matrix. 
 */
class MeshModel
{
protected:
	// Protected members
	std::vector<Face> faces;
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	glm::mat4x4 worldTransform;
	
	// User inputs
	glm::vec3 translationVector;
	int scaleSize;
	int rotateAngle;

	// Member functions that return transformation matrices
	glm::mat4x4 GetTranslationMatrix();
	glm::mat4x4 GetScalingMatrix();
	glm::mat4x4 GetRotateMatrix();
	
	// Stuff that I haven't figured out what to do with yet
	glm::vec4 color;
	std::string modelName;
	glm::vec3 centerPoint;
	glm::vec3 minimums;
	glm::vec3 maximums;

public:
	MeshModel(const std::vector<Face>& faces, const std::vector<glm::vec3>& vertices, const std::vector<glm::vec3>& normals, const std::string& modelName = "");
	virtual ~MeshModel();

	// Setters
	void SetColor(const glm::vec4& color);
	void SetTranslation(glm::vec3 direction) { translationVector = direction; }
	void SetScaleSize(int scale) {scaleSize = scale; }
	void SetRotationAngle(int angle) { rotateAngle = angle; }

	//Getters
	std::vector<glm::vec3>& GetVerticesVector() { return vertices; }
	std::vector<Face>& GetFacesVector() { return faces; }
	glm::vec3& GetCenterPoint()	   { return centerPoint; }
	glm::vec3& GetMinimumsVector() { return minimums; }
	glm::vec3& GetMaximumVectors() { return maximums; }
	const glm::mat4x4& GetWorldTransformation();
	const glm::vec4& GetColor() const;
	const std::string& GetModelName();
	glm::vec3& GetTranslationVector() { return translationVector; }
};
