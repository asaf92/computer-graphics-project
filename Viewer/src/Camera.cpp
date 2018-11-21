#define _USE_MATH_DEFINES

#include "Camera.h"
#include "Utils.h"
#include <glm/gtc/type_ptr.hpp>
#include <iostream>


Camera::Camera() : Camera::Camera(glm::vec3(0, 0,-3),
								  glm::vec3(0, 0, 0),
								  glm::vec3(0, 1, 0))
{

}

Camera::Camera(const glm::vec3& eye, const glm::vec3& at, const glm::vec3& up) :
	zoom(1.0)
{
	SetCameraLookAt(eye, at, up);
	//SetPerspectiveProjection(60.0f, 4.0f / 3.0f, 0.1f, 9.0f);
	SetOrthographicProjection(-2.0f,2.0f,-2.0f,2.0f,0.1f,2.0f);
}

Camera::~Camera()
{
}

void Camera::SetCameraLookAt(const glm::vec3& eye, const glm::vec3& at, const glm::vec3& up)
{
	cameraParameters[0] = eye;
	cameraParameters[1] = at;
	cameraParameters[2] = up;
	
	// In order to keep looking straight:
	// cameraParameters[1][2] = cameraParameters[0][2];

	glm::vec3 straight = glm::normalize(at - eye);
	glm::vec3 upVector = glm::normalize(up);
	glm::vec3 right = glm::normalize(glm::cross(up, straight));
	upVector = glm::normalize(glm::cross(straight,right));

	glm::mat4x4 viewMatrix(1);
	viewMatrix[0][0] = right.x;
	viewMatrix[0][1] = right.y;
	viewMatrix[0][2] = right.z;
	viewMatrix[0][3] = glm::dot(-right, eye);
	viewMatrix[1][0] = upVector.x;
	viewMatrix[1][1] = upVector.y;
	viewMatrix[1][2] = upVector.z;
	viewMatrix[1][3] = glm::dot(-upVector, eye);
	viewMatrix[2][0] = -straight.x;
	viewMatrix[2][1] = -straight.y;
	viewMatrix[2][2] = -straight.z;
	viewMatrix[2][3] = glm::dot(-straight, eye);
	viewMatrix[3][0] = 0.0f;
	viewMatrix[3][1] = 0.0f;
	viewMatrix[3][2] = 0.0f;
	viewMatrix[3][3] = 1.0f;

	viewTransformation = viewMatrix;
}

void Camera::SetOrthographicProjection(float left, float right, float top, float bottom, float zNear, float zFar)
{
	IsOrthographic = true;
	projectionTransformation = glm::mat4x4(1);


		/*glm::mat4x4(
		{

			{2.0f / (right - left) ,0,0,0},
			{0,2.0f / (top - bottom) ,0,0},
			{0,0,-2.0f / (zFar - zNear),0},
			{ -(right + left) / (right - left) , -(bottom + top) / (top - bottom) , -(zFar + zNear) / (zFar - zNear),1}

		});*/
}

void Camera::SetPerspectiveProjection(
	const float fovy,
	const float aspectRatio,
	const float near,
	const float far)
{
	IsOrthographic = false;
	ProjectionValues[0] = fovy;
	ProjectionValues[1]= aspectRatio;
	ProjectionValues[2]= near;
	ProjectionValues[3]= far;

	float fov = fovy * 0.01745329251994329576923690768489f; // pi/180
	
	float tanHalfFovy = tan(fov / 2.0f);

	glm::mat4x4 result(0);

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