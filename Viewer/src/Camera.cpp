#define _USE_MATH_DEFINES

#include "Camera.h"
#include <glm/gtc/type_ptr.hpp>
#include <iostream>


Camera::Camera() : Camera::Camera(glm::vec4(0, 0, 1, 0),
								  glm::vec4(0, 0, 0, 0),
								  glm::vec4(0, 1, 0, 0))
{

}

Camera::Camera(const glm::vec4& eye, const glm::vec4& at, const glm::vec4& up) :
	zoom(50.0)
{
	SetCameraLookAt(eye, at, up);
}

Camera::~Camera()
{
}

void Camera::SetCameraLookAt(const glm::vec3& eye, const glm::vec3& at, const glm::vec3& up)
{
	glm::vec3 zAxis = glm::normalize(eye - at);
	glm::vec3 xAxis = glm::normalize(glm::cross(zAxis,up));
	glm::vec3 yAxis = cross(xAxis, zAxis);

	zAxis = zAxis * -1.0f;

	glm::mat4 viewMatrix = {
		glm::vec4(xAxis.x,xAxis.y,xAxis.z,-glm::dot(xAxis,eye)),
		glm::vec4(yAxis.x,yAxis.y,yAxis.z,-glm::dot(yAxis,eye)),
		glm::vec4(zAxis.x,zAxis.y,zAxis.z,-glm::dot(zAxis,eye)),
		glm::vec4(0,0,0,1)
	};

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
	projectionTransformation = glm::mat4x4(1); // For now make it the identity
}

