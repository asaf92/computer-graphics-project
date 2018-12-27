#pragma once
#include <glm/glm.hpp>
#include <string>
#include <memory>
#include "Face.h"
#include "Material.h"
/*
 * MeshModel class.
 * This class represents a mesh model (with faces and normals informations).
 * You can use Utils::LoadMeshModel to create instances of this class from .obj files.
 *
 * The way I implement this class is that the renderer will feed the move, rotate & scale inputs to 
 * the object and the getWorldTransform will return the 4x4 model->world transformation matrix. 
 *
 * Made by Asaf Agami 2018
 */
class MeshModel
{
protected:
	// Protected members
	std::vector<Face> faces;
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	glm::mat4x4 worldTransform;
	glm::mat4x4 rotateTransformation;
	std::string modelName;
	Material uniformMaterial;

	// User inputs
	glm::vec3 translationVector;
	glm::vec3 scaleSize;
	glm::vec4 color;
	glm::vec3 rotateAngle;

	// Member functions that return transformation matrices
	glm::mat4x4 GetTranslationMatrix();
	glm::mat4x4 GetScalingMatrix();
	glm::mat4x4 GetRotationMatrix();
	glm::mat4x4 GetXRotationMatrix();
	glm::mat4x4 GetYRotationMatrix();
	glm::mat4x4 GetZRotationMatrix();
	
	// Necessary for rotations
	glm::vec3 centerPoint;

	// Stuff that I haven't figured out what to do with yet
	glm::vec3 minimums;
	glm::vec3 maximums;

public:
	MeshModel(const std::vector<Face>& faces, const std::vector<glm::vec3>& vertices, const std::vector<glm::vec3>& normals, std::string& name) : MeshModel(faces, vertices, normals, glm::vec4(0.9f,0.9f,0.0f,1.0f), name) {}
	MeshModel(const std::vector<Face>& faces, const std::vector<glm::vec3>& vertices, const std::vector<glm::vec3>& normals, const glm::vec4& color) : MeshModel(faces, vertices, normals, color, std::string("")) {}
	MeshModel(const std::vector<Face>& faces, const std::vector<glm::vec3>& vertices, const std::vector<glm::vec3>& normals, const glm::vec4& color, std::string& name );
	virtual ~MeshModel();

	// Setters
	void SetColor(const glm::vec4& color)              { this->color = color; }
	void SetTranslation(glm::vec3 direction)           { translationVector = direction; }
	void SetScaling(glm::vec3 scale)                   { scaleSize         = scale; }
	void SetObjectDiffuseColor(const glm::vec4& color) { uniformMaterial.SetDiffuseColor(color); }
	void SetRotation(const glm::vec3& angle);

	//Getters
	std::vector<glm::vec3>& GetVerticesVector()             { return vertices; }
	const std::vector<glm::vec3>& GetNormalsVector()        { return normals; }
	std::vector<Face>& GetFacesVector() { return faces; }
	const glm::mat4x4& GetWorldTransformation();
	const glm::vec4& GetColor()                      const;
	const std::string& GetModelName()                const  { return modelName; }
	const glm::vec3& GetTranslationVector()	         const  { return translationVector; }
	const glm::vec3& GetScalingVector()              const  { return scaleSize; }
	Material& GetUniformMaterial()                          { return uniformMaterial; }
	glm::vec4 GetDiffuseColor()                      const	{ return glm::vec4(uniformMaterial.GetDiffuseColor()); }

	// Not sure if needed
	glm::vec3& GetCenterPoint()						        { return centerPoint; }
	glm::vec3& GetMinimumsVector()					        { return minimums; }
	glm::vec3& GetMaximumVectors()					        { return maximums; }
};
