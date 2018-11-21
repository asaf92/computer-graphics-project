#pragma once
#include <memory>
#include <glm/glm.hpp>
#include "MeshModel.h"

/*
 * Camera class. This class takes care of all the camera transformations and manipulations.
 *
 * Implementation suggestion:
 * --------------------------
 * Make the Camera class be a subclass of MeshModel, so you can easily and elegantly render 
 * the cameras you have added to the scene.
 */
class Camera
{
private:
	glm::mat3x3 cameraParameters;
	glm::mat4x4 viewTransformation;
	glm::mat4x4 projectionTransformation;
	glm::vec4 ProjectionValues;
	float zoom;

public:
	Camera();
	Camera(const glm::vec3& eye, const glm::vec3& at, const glm::vec3& up);
	~Camera();

	// Sets the viewTransformation matrix
	void SetCameraLookAt(const glm::vec3& eye, const glm::vec3& at, const glm::vec3& up);
	glm::mat3x3& GetCameraParameters() { return cameraParameters; }
	void SetOrthographicProjection(float left, float right, float top, float bottom, float zNear, float zFar);

	void SetPerspectiveProjection(
		const float fovy,
		const float aspect,
		const float near,
		const float far);

	glm::mat4x4 CreateFrustum(float left, float right, float top, float bottom, float, float);

	void SetZoom(const float zoom) { this->zoom = zoom; };
	float GetZoom() const { return zoom; }
	// Add more methods/functionality as needed...

	//Getters
	const glm::mat4x4& GetViewMatrix() const { return viewTransformation; };
	const glm::mat4x4& GetProjectionMatrix() const { return projectionTransformation; }

	// Projection Values
	float GetFoV()			{ return ProjectionValues[0]; }
	float GetAspectRatio()	{ return ProjectionValues[1]; }
	float GetNear()			{ return ProjectionValues[2]; }
	float GetFar()			{ return ProjectionValues[3]; }

	bool IsOrthographic;
};

struct ProjectionParameters {
	float fov;
	float aspect;
	float zNear;
	float zFar;
};