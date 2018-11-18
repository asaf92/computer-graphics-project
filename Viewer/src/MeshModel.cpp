#include "MeshModel.h"
#include "Utils.h"
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>

MeshModel::MeshModel(const std::vector<Face>& faces, const std::vector<glm::vec3>& vertices, const std::vector<glm::vec3>& normals, const std::string& modelName) :
	faces(faces),
	vertices(vertices),
	normals(normals),
	modelName(modelName),
	worldTransform(glm::mat4x4(1)),
	scaleSize(1),
	rotateAngle(0),
	translationVector(0),
	minimums(0),
	maximums(0),
	centerPoint(0)
{
	for (std::vector<glm::vec3>::const_iterator iterator = vertices.cbegin(); iterator != vertices.end(); ++iterator)
	{
		// Init the minimums and maximums vector
		minimums.x = std::min(minimums.x, iterator->x);
		minimums.y = std::min(minimums.y, iterator->y);
		minimums.z = std::min(minimums.z, iterator->z);
		maximums.x = std::max(maximums.x, iterator->x);
		maximums.y = std::max(maximums.y, iterator->y);
		maximums.z = std::max(maximums.z, iterator->z);

		centerPoint.x += iterator->x;
		centerPoint.y += iterator->y;
		centerPoint.z += iterator->z;
	}
	centerPoint.x = centerPoint.x / vertices.size();
	centerPoint.y = centerPoint.y / vertices.size();
	centerPoint.z = centerPoint.z / vertices.size();
}

MeshModel::~MeshModel()
{

}

// AKA translate
glm::mat4x4 MeshModel::GetTranslationMatrix() 
{
	glm::mat4x4 translate(1);
	translate[3].x = translationVector.x;
	translate[3].y = translationVector.y;
	translate[3].z = translationVector.z;
	return translate;
}

glm::mat4x4 MeshModel::GetScalingMatrix() 
{
	glm::mat4x4 scale(1);
	scale[0].x *= scaleSize;
	scale[1].y *= scaleSize;
	scale[2].z *= scaleSize;
	return scale;
}

glm::mat4x4 MeshModel::GetRotateMatrix()
{
	// TODO- Implement
	// For now just return Identity
	return glm::mat4x4(1);
}

const glm::mat4x4& MeshModel::GetWorldTransformation()
{
	worldTransform = glm::mat4x4(1);
	glm::mat4x4 translate = GetTranslationMatrix();
	glm::mat4x4 rotate	  = GetRotateMatrix();
	glm::mat4x4 scale     = GetScalingMatrix();

	worldTransform = translate * rotate * scale;
	return worldTransform;
}

void MeshModel::SetColor(const glm::vec4& color)
{
	this->color = color;
}

const glm::vec4& MeshModel::GetColor() const
{
	return color;
}

const std::string& MeshModel::GetModelName()
{
	return modelName;
}