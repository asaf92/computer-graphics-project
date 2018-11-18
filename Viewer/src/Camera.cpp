#define _USE_MATH_DEFINES

#include "Camera.h"
#include <glm/gtc/type_ptr.hpp>
#include <iostream>


Camera::Camera() : Camera::Camera(glm::vec4(0, 0, 1, 0),
								  glm::vec4(0, 0, -1, 1),
								  glm::vec4(0, 1, 0, 0))
{

}

Camera::Camera(const glm::vec4& eye, const glm::vec4& at, const glm::vec4& up) :
	zoom(1.0)
{
	SetCameraLookAt(eye, at, up);
	projectionTransformation = glm::mat4x4(1);
}

Camera::~Camera()
{
}

void Camera::SetCameraLookAt(const glm::vec3& eye, const glm::vec3& at, const glm::vec3& up)
{
	glm::vec3 straight = glm::normalize(at - eye);
	glm::vec3 upVector = glm::normalize(up);
	glm::vec3 right = glm::normalize(glm::cross(straight, up));
	upVector = glm::cross(straight, right);

	glm::mat4x4 viewMatrix(1);
	viewMatrix[0][0] = right.x;
	viewMatrix[1][0] = right.y;
	viewMatrix[2][0] = right.z;
	viewMatrix[0][1] = upVector.x;
	viewMatrix[1][1] = upVector.y;
	viewMatrix[2][1] = upVector.z;
	viewMatrix[0][2] = -straight.x;
	viewMatrix[1][2] = -straight.y;
	viewMatrix[2][2] = -straight.z;
	viewMatrix[3][0] = -glm::dot(right, eye);
	viewMatrix[3][1] = -glm::dot(upVector, eye);
	viewMatrix[3][2] = glm::dot(straight, eye);
	
	viewTransformation = viewMatrix;
}

void Camera::SetOrthographicProjection(
	const float height,
	const float aspectRatio,
	const float near,
	const float far)
{

}

void Camera::SetPerspectiveProjection(
	const float fovy,
	const float aspectRatio,
	const float near,
	const float far)
{
	projectionTransformation = glm::mat4x4(1);
}

