#define _USE_MATH_DEFINES

#include "Camera.h"
#include <glm/gtc/type_ptr.hpp>
#include <iostream>


Camera::Camera() : Camera::Camera(glm::vec3(0, 0,-1),
								  glm::vec3(0, 0, 0),
								  glm::vec3(0, -1, 0))
{

}

Camera::Camera(const glm::vec3& eye, const glm::vec3& at, const glm::vec3& up) :
	zoom(1.0)
{
	SetCameraLookAt(eye, at, up);
	SetPerspectiveProjection(90.0f, 4.0f / 3.0f, 0.1f, 1.0f);
	//SetOrthographicProjection(60.0f, 4.0f / 3.0f, 0.1f, 2.0f);
}

Camera::~Camera()
{
}

void Camera::SetCameraLookAt(const glm::vec3& eye, const glm::vec3& at, const glm::vec3& up)
{
	glm::vec3 straight = glm::normalize(at- eye);
	glm::vec3 upVector = glm::normalize(up);
	glm::vec3 right = glm::normalize(glm::cross(up, straight));
	upVector = glm::cross(straight, right);
	upVector = glm::normalize(upVector);

	glm::mat4x4 viewMatrix(1);
	viewMatrix[0][0] = right.x;
	viewMatrix[0][1] = right.y;
	viewMatrix[0][2] = right.z;
	viewMatrix[0][3] = glm::dot(-right, eye);
	viewMatrix[1][0] = upVector.x;
	viewMatrix[1][1] = upVector.y;
	viewMatrix[1][2] = upVector.z;
	viewMatrix[1][3] = glm::dot(-upVector, eye);
	viewMatrix[2][0] = straight.x;
	viewMatrix[2][1] = straight.y;
	viewMatrix[2][2] = straight.z;
	viewMatrix[2][3] = glm::dot(-straight, eye);
	viewMatrix[3][0] = 0.0f;
	viewMatrix[3][1] = 0.0f;
	viewMatrix[3][2] = 0.0f;
	viewMatrix[3][3] = 1.0f;

	viewTransformation = viewMatrix;
}

void Camera::SetOrthographicProjection(
	const float height,
	const float aspectRatio,
	const float near,
	const float far)
{
	projectionTransformation = glm::mat4x4(1);
}

void Camera::SetPerspectiveProjection(
	const float fovy,
	const float aspectRatio,
	const float near,
	const float far)
{
	projectionTransformation = glm::mat4x4(1);
}
