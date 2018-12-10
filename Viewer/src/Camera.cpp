#define _USE_MATH_DEFINES

#include "Camera.h"
#include "Utils.h"
#include <glm/gtc/type_ptr.hpp>
#include <iostream>


Camera::Camera() : Camera::Camera(glm::vec3(3, 3,-3),
								  glm::vec3(0, 0, 0),
								  glm::vec3(0, 1, 0))
{

}

Camera::Camera(const glm::vec3& eye, const glm::vec3& at, const glm::vec3& up) :
	zoom(1.0)
{
	orthographicProjectionParameters = { -1.0f ,1.0f ,-1.0f ,1.0f ,-1.0f ,1.0f };
	perspectiveProjectionParameters = { 60.0f, 4 / 3, 1.0f,4.0f };
	SetCameraLookAt(eye, at, up);
	projectionTransformation = glm::mat4x4(1);
	activeProjectionType = None;
}

Camera::~Camera()
{
}

void Camera::SetCameraLookAt(const glm::vec3& eye, const glm::vec3& at, const glm::vec3& up)
{
	lookAtParameters.eye = eye;
	lookAtParameters.at = at;
	lookAtParameters.up = up;

	glm::vec3 straight = glm::normalize(at - eye);
	glm::vec3 right = glm::normalize(glm::cross(up, straight));
	glm::vec3 upVector = glm::normalize(glm::cross(straight,right));

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
	viewMatrix[3][0] = 0.0f;
	viewMatrix[3][1] = 0.0f;
	viewMatrix[3][2] = 0.0f;
	viewMatrix[3][3] = 1.0f;
	viewMatrix[0][3] = -glm::dot(right, eye);
	viewMatrix[1][3] = -glm::dot(upVector, eye);
	viewMatrix[2][3] = -glm::dot(straight, eye);

	viewTransformation = viewMatrix;
}

void Camera::SetOrthographicProjection()
{
	SetOrthographicProjection(orthographicProjectionParameters.left, 
							  orthographicProjectionParameters.right, 
							  orthographicProjectionParameters.top, 
							  orthographicProjectionParameters.bottom, 
							  orthographicProjectionParameters.zNear, 
							  orthographicProjectionParameters.zFar
	);
}

void Camera::SetOrthographicProjection(float left, float right, float top, float bottom, float zNear, float zFar)
{
	projectionTransformation = glm::mat4x4(
		{

			{2.0f / (right - left) ,0,0,0},
			{0,2.0f / (top - bottom) ,0,0},
			{0,0,-2.0f / (zFar - zNear),0},
			{ -(right + left) / (right - left) , -(bottom + top) / (top - bottom) , -(zFar + zNear) / (zFar - zNear),1} //http://learnwebgl.brown37.net/08_projections/projections_ortho.html

		});
}

void Camera::SetPerspectiveProjection()
{
	SetPerspectiveProjection(perspectiveProjectionParameters.fov, perspectiveProjectionParameters.aspect, perspectiveProjectionParameters.zNear, perspectiveProjectionParameters.zFar);
}

void Camera::SetPerspectiveProjection(
	const float fovy,
	const float aspectRatio,
	const float near,
	const float far)
{
	perspectiveProjectionParameters.fov = fovy;
	perspectiveProjectionParameters.aspect= aspectRatio;
	perspectiveProjectionParameters.zNear= near;
	perspectiveProjectionParameters.zFar= far;

	float fov = fovy * 0.01745329251994329576923690768489f; // pi/180
	
	float tanHalfFovy = tan(fov / 2.0f);

	glm::mat4x4 result(1);

	result[0][0] = 1.0f / (aspectRatio * tanHalfFovy);
	result[1][1] = 1.0f / (tanHalfFovy);
	result[2][2] = (far + near) / (near - far);
	result[2][3] = -1.0f;
	result[3][2] = (2.0f * far * near) / (near - far);
	
	projectionTransformation = result;
	//float height = near * glm::tan(glm::radians(fovy)) / 2.0f;
	//float width = height * aspectRatio;
	//float left = -width;
	//float right = width;
	//float bottom = -height;
	//float top = height;
	//float zNear = near;
	//float zFar = far;

	//projectionTransformation = CreateFrustum(left, right, top, bottom, zNear, zFar);
}

glm::mat4x4 Camera::CreateFrustum(float left, float right, float top, float bottom, float near, float far)
{
	return glm::mat4x4(
		{2.0f*near/(right- left),0,0,0},
		{0,2.0f*near/(top-bottom),0,0},
		{(right + left)/(right - left), (top + bottom) / (top - bottom), -(far+near)/(far - near), -1 },
		{0,0,-2.0f*far*near / (far-near),0}
	);
}

void Camera::RenderProjectionMatrix()
{
	switch (activeProjectionType)
	{
	case Perspective:
		SetPerspectiveProjection();
		break;
	case Ortographic:
		SetOrthographicProjection();
		break;
	case None:
		projectionTransformation = glm::mat4x4(1);
	}
}

const OrthographicProjectionParameters Camera::GetOrthographicProjectionParameters() const
{
	return orthographicProjectionParameters;
}

void Camera::SetOrthographicProjectionParameters(OrthographicProjectionParameters parameters)
{
	orthographicProjectionParameters = parameters;
}
