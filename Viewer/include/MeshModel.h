#pragma once
#include <glm/glm.hpp>
#include <glad/glad.h>
#include <string>
#include <memory>
#include "ShadingModels.h"
#include "Face.h"
#include "Material.h"
#include "Vertex.h"
#include "IMovable.h"
#include "IRotatable.h"
#include "IMeshObject.h"
#include "IScalable.h"
#include "IUniformMaterial.h"
#include "Texture2D.h"

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
class MeshModel: public IMovable, public IRotatable, public IMeshObject, public IScalable, public IUniformMaterial
{
protected:
	// Protected members
	glm::mat4x4 worldTransform;
	glm::mat4x4 rotateTransformation;
	std::string modelName;
	Material uniformMaterial;
	ShadingModels shadingModel;

	// User inputs
	glm::vec3 translationVector;
	glm::vec3 scaleSize;
	glm::vec4 color;
	glm::vec3 rotateAngle;

	// Member functions that return transformation matrices
	glm::mat4x4 GetRotationMatrix()    const;
	glm::mat4x4 GetTranslationMatrix() const;
	glm::mat4x4 GetScalingMatrix()     const;
	glm::mat4x4 GetXRotationMatrix()   const;
	glm::mat4x4 GetYRotationMatrix()   const;
	glm::mat4x4 GetZRotationMatrix()   const;
	
	// Necessary for rotations
	glm::vec3 centerPoint;

	// Stuff that I haven't figured out what to do with yet
	glm::vec3 minimums;
	glm::vec3 maximums;
	glm::vec3& GetCenterPoint()						        { return centerPoint; }
	glm::vec3& GetMinimumsVector()					        { return minimums; }
	glm::vec3& GetMaximumVectors()					        { return maximums; }

	// Texture
	Texture2D texture;
	bool textureLoaded;

	// OpenGL stuff
	glm::mat4x4 modelTransform;
	std::vector<Vertex> modelVertices;
	GLuint vao;
	GLuint vbo;
	
	// Bump mapping
	Texture2D* bumpMap;

public:
	// ctors
	MeshModel() {}
	MeshModel(std::vector<Face> faces, std::vector<glm::vec3> vertices, const std::string& modelName);
	MeshModel(std::vector<Face> faces, std::vector<glm::vec3> vertices, std::vector<glm::vec2> textureCoords, const std::string& modelName, const std::string& textureFileName);
	MeshModel(std::vector<Face> faces, std::vector<glm::vec3> vertices, std::vector<glm::vec3> normals, std::vector<glm::vec2> textureCoords, const std::string & modelName); 
	virtual ~MeshModel();

	// Setters
	void SetTranslation(glm::vec3 direction)            { translationVector = direction; }
	void SetRotation(const glm::vec3& angle);

	// Getters
	const std::string& GetModelName()                const  { return modelName; }
	const glm::vec3& GetTranslationVector()	         const  { return translationVector; }
	Material& GetUniformMaterial()                          { return uniformMaterial; }

	// Textures
	void BindTextures()  const                                    { texture.bind(0); }
	void UnbindTextures() const                                   { texture.unbind(0); }
	const bool TextureLoaded() const                              { return textureLoaded; }

	Texture2D* GetBumpMap() const { return bumpMap; }

	#pragma region Interfaces Implementations
	// Inherited via IMovable
	virtual void Move(const glm::vec3 direction) override { SetTranslation(translationVector + direction); }

	// Inherited via IRotatable
	virtual void RotateX(const float angle) override;
	virtual void RotateY(const float angle) override;
	virtual void RotateZ(const float angle) override;

	// Inherited via IMeshObject
	virtual const GLuint&      GetVao() const override { return vao; }
	virtual const unsigned int GetNumberOfVertices() const override { return modelVertices.size(); }
	virtual const glm::mat4 GetWorldTransformation() const;
	virtual const glm::mat4 GetModelTransformation() const { return glm::mat4(1.0f); }
	
	// Inherited via IScalable
	virtual void Scale(const float scale)      override                         { scaleSize         = glm::vec3(scale); }
	virtual void Scale(const glm::vec3& scale) override                         { scaleSize         = scale; }
	const glm::vec3& GetScale()         const  override { return scaleSize; }

	// Inherited via IUniformMaterial
	virtual const glm::vec4 GetAmbientColor()                const override;
	virtual const glm::vec4 GetDiffuseColor()                const override { return glm::vec4(uniformMaterial.GetDiffuseColor()); }
	virtual const glm::vec4 GetSpecularColor()               const override { return glm::vec4(uniformMaterial.GetSpecularColor()); }
	virtual const float GetShininess()                       const override { return uniformMaterial.GetShininess(); }
	virtual void SetAmbientColor(const glm::vec4& color)  override { this->color = color; }
	virtual void SetDiffuseColor(const glm::vec4& color)  override { uniformMaterial.SetDiffuseColor(color); }
	virtual void SetSpecularColor(const glm::vec4& color) override { uniformMaterial.SetSpecularColor(color); }
	virtual void SetShininess(const float shininess)      override { uniformMaterial.SetShininess(shininess); }
	#pragma endregion 
};
