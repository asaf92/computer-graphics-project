#include "MeshModel.h"
#include "Utils.h"
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <math.h>

MeshModel::MeshModel(const std::vector<Face>& faces, const std::vector<glm::vec3>& vertices, const std::vector<glm::vec3>& normals, const std::string& modelName) :
	faces(faces),
	vertices(vertices),
	normals(normals),
	modelName(modelName),
	worldTransform(glm::mat4x4(1)),
	scaleSize(1.0f, 1.0f, 1.0f),
	rotateAngle(0.0f),
	rotateTransformation(1),
	translationVector(glm::vec3(0.0f,0.0f,4.0f)),
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
	scale[0].x *= scaleSize.x;
	scale[1].y *= scaleSize.y;
	scale[2].z *= scaleSize.z;
	return scale;
}

glm::mat4x4 MeshModel::GetRotationMatrix()
{
	if (rotateAngle.x != 0)
	{
		return rotateTransformation = GetXRotationMatrix() * rotateTransformation;
	}
	if (rotateAngle.y != 0)
	{
		return rotateTransformation = GetYRotationMatrix() * rotateTransformation;
	}
	if (rotateAngle.z != 0)
	{
		return rotateTransformation = GetZRotationMatrix() * rotateTransformation;
	}
	return rotateTransformation;
}

glm::mat4x4 MeshModel::GetXRotationMatrix()
{
	float pi = atan(1) * 4;

	glm::mat4x4 rotationMatrix(1);
	glm::mat4x4 translateToCenter(1);
	translateToCenter[3].x = -centerPoint.x;
	translateToCenter[3].y = -centerPoint.y;
	translateToCenter[3].z = -centerPoint.z;
	glm::mat4x4 translateToCenterInverse(1);
	translateToCenterInverse[3].x = centerPoint.x;
	translateToCenterInverse[3].y = centerPoint.y;
	translateToCenterInverse[3].z = centerPoint.z;
	float rotateAngleRadians = rotateAngle.x * pi / 180;

	rotationMatrix[1][1] = cos(rotateAngleRadians);
	rotationMatrix[2][2] = cos(rotateAngleRadians);
	rotationMatrix[2][1] = -sin(rotateAngleRadians);
	rotationMatrix[1][2] = sin(rotateAngleRadians);

	return translateToCenterInverse * rotationMatrix * translateToCenter;
}

glm::mat4x4 MeshModel::GetYRotationMatrix()
{
	float pi = atan(1) * 4;
	
	glm::mat4x4 rotationMatrix(1);
	glm::mat4x4 translateToCenter(1);
	translateToCenter[3].x = -centerPoint.x;
	translateToCenter[3].y = -centerPoint.y;
	translateToCenter[3].z = -centerPoint.z;
	glm::mat4x4 translateToCenterInverse(1);
	translateToCenterInverse[3].x = centerPoint.x;
	translateToCenterInverse[3].y = centerPoint.y;
	translateToCenterInverse[3].z = centerPoint.z;
	float rotateAngleRadians = rotateAngle.y * pi / 180;

	rotationMatrix[0][0] = cos(rotateAngleRadians);
	rotationMatrix[2][2] = cos(rotateAngleRadians);
	rotationMatrix[0][2] = -sin(rotateAngleRadians);
	rotationMatrix[2][0] = sin(rotateAngleRadians);

	return translateToCenterInverse * rotationMatrix * translateToCenter;
}

glm::mat4x4 MeshModel::GetZRotationMatrix()
{
	float pi = atan(1) * 4;

	glm::mat4x4 rotationMatrix(1);
	glm::mat4x4 translateToCenter(1);
	translateToCenter[3].x = -centerPoint.x;
	translateToCenter[3].y = -centerPoint.y;
	translateToCenter[3].z = -centerPoint.z;
	glm::mat4x4 translateToCenterInverse(1);
	translateToCenterInverse[3].x = centerPoint.x;
	translateToCenterInverse[3].y = centerPoint.y;
	translateToCenterInverse[3].z = centerPoint.z;
	float rotateAngleRadians = rotateAngle.z * pi / 180;

	rotationMatrix[0][0] = cos(rotateAngleRadians);
	rotationMatrix[1][1] = cos(rotateAngleRadians);
	rotationMatrix[1][0] = -sin(rotateAngleRadians);
	rotationMatrix[0][1] = sin(rotateAngleRadians);

	return translateToCenterInverse * rotationMatrix * translateToCenter;
}

const glm::mat4x4& MeshModel::GetWorldTransformation()
{
	worldTransform = glm::mat4x4(1);
	glm::mat4x4 translate = GetTranslationMatrix();
	glm::mat4x4 rotate	  = GetRotationMatrix();
	rotateAngle = glm::vec3(0.0f);
	glm::mat4x4 scale     = GetScalingMatrix();

	worldTransform = translate * rotate * scale;
	return worldTransform;
}

void MeshModel::SetColor(const glm::vec4& color)
{
	this->color = color;
}

void MeshModel::SetRotation(const glm::vec3 & angle)
{
	if (angle.x != 0)
	{
		rotateAngle.x = angle.x;
		return;
	}
	if (angle.y != 0)
	{
		rotateAngle.y = angle.y;
		return;
	}
	if (angle.z != 0)
	{
		rotateAngle.z = angle.z;
		return;
	}
}

void MeshModel::Rotate(glm::vec3)
{

}

const glm::vec4& MeshModel::GetColor() const
{
	return color;
}
