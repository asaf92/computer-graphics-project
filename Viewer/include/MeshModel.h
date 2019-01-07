#pragma once
#include <glm/glm.hpp>
#include <string>
#include <memory>
#include "Face.h"
#include "Material.h"
#include "IMovable.h"
#include "IRotatable.h"
#include "IMeshObject.h"
#include "IScalable.h"

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
class MeshModel: public IMovable, public IRotatable, public IMeshObject, public IScalable
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
	glm::vec3& GetCenterPoint()						        { return centerPoint; }
	glm::vec3& GetMinimumsVector()					        { return minimums; }
	glm::vec3& GetMaximumVectors()					        { return maximums; }

public:
	MeshModel(const std::vector<Face>& faces, const std::vector<glm::vec3>& vertices, const std::vector<glm::vec3>& normals, std::string& name) : MeshModel(faces, vertices, normals, glm::vec4(0.1f,0.1f,0.1f,1.0f), name) {}
	MeshModel(const std::vector<Face>& faces, const std::vector<glm::vec3>& vertices, const std::vector<glm::vec3>& normals, const glm::vec4& color) : MeshModel(faces, vertices, normals, color, std::string("")) {}
	MeshModel(const std::vector<Face>& faces, const std::vector<glm::vec3>& vertices, const std::vector<glm::vec3>& normals, const glm::vec4& color, std::string& name );
	virtual ~MeshModel();

	// Setters
	void SetColor(const glm::vec4& color)               { this->color = color; }
	void SetTranslation(glm::vec3 direction)            { translationVector = direction; }
	void SetObjectDiffuseColor(const glm::vec4& color)  { uniformMaterial.SetDiffuseColor(color); }
	void SetObjectSpecularColor(const glm::vec4& color) { uniformMaterial.SetSpecularColor(color); }
	void SetRotation(const glm::vec3& angle);

	//Getters
	const std::string& GetModelName()                const  { return modelName; }
	const glm::vec3& GetTranslationVector()	         const  { return translationVector; }
	const glm::vec3& GetScalingVector()              const  { return scaleSize; }
	Material& GetUniformMaterial()                          { return uniformMaterial; }

	// Inherited via IMovable
	virtual void Move(const glm::vec3 direction) override { SetTranslation(translationVector + direction); }

	// Inherited via IRotatable
	virtual void RotateX(const float angle) override;
	virtual void RotateY(const float angle) override;
	virtual void RotateZ(const float angle) override;

	// Inherited via IMeshObject
	virtual std::vector<glm::vec3>& GetVerticesVector()             override { return vertices; }
	virtual const std::vector<glm::vec3>& GetNormalsVector()        override { return normals; }
	virtual std::vector<Face>& GetFacesVector()				        override { return faces; }
	const glm::mat4x4& GetWorldTransformation()				        override;
	virtual const glm::vec4 GetAmbientColor()                const override;
	virtual const glm::vec4 GetDiffuseColor()                const override { return glm::vec4(uniformMaterial.GetDiffuseColor()); }
	virtual const glm::vec4 GetSpecularColor()               const override { return glm::vec4(uniformMaterial.GetSpecularColor()); }

	// Inherited via IScalable
	virtual void Scale(const float scale)      override                         { scaleSize         = glm::vec3(scale); }
	virtual void Scale(const glm::vec3& scale) override                         { scaleSize         = scale; }
};
