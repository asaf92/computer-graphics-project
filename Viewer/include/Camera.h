#pragma once
#include <memory>
#include <glm/glm.hpp>
#include "MeshModel.h"
#include "ProjectionType.h"

/*
 * Camera class. This class takes care of all the camera transformations and manipulations.
 *
 * Implementation suggestion:
 * --------------------------
 * Make the Camera class be a subclass of MeshModel, so you can easily and elegantly render 
 * the cameras you have added to the scene.
 */

struct PerspectiveProjectionParameters {
	float fov;
	float aspect;
	float zNear;
	float zFar;
};

struct OrthographicProjectionParameters {
	float left;
	float right;
	float bottom;
	float top;
	float zNear;
	float zFar;
};

class Camera
{
private:
	glm::mat3x3 cameraParameters;
	glm::mat4x4 viewTransformation;
	glm::mat4x4 projectionTransformation;
	glm::vec4 ProjectionValues;
	OrthographicProjectionParameters orthographicProjectionParameters;
	ProjectionType activeProjectionType;
	float zoom;
	void SetOrthographicProjection(float left, float right, float top, float bottom, float zNear, float zFar);
	glm::mat4x4 CreateFrustum(float left, float right, float top, float bottom, float, float);

public:
	Camera();
	Camera(const glm::vec3& eye, const glm::vec3& at, const glm::vec3& up);
	~Camera();

	// Sets the viewTransformation matrix
	void SetCameraLookAt(const glm::vec3& eye, const glm::vec3& at, const glm::vec3& up);
	void SetOrthographicProjection();
	glm::mat3x3& GetCameraParameters() { return cameraParameters; }
	void SetPerspectiveProjection();
	void RenderProjectionMatrix();
	void SetZoom(const float zoom) { this->zoom = zoom; };
	float GetZoom() const { return zoom; }

	const glm::mat4x4& GetViewMatrix() const { return viewTransformation; }
	const glm::mat4x4& GetProjectionMatrix() const { return projectionTransformation; }

	// Projection
	void SelectProjectionType(ProjectionType selection) { activeProjectionType = selection; }
	const ProjectionType GetProjectionType() const { return activeProjectionType; }

	float GetFoV()			{ return ProjectionValues[0]; }
	float GetAspectRatio()	{ return ProjectionValues[1]; }
	float GetNear()			{ return ProjectionValues[2]; }
	float GetFar()			{ return ProjectionValues[3]; }

	const OrthographicProjectionParameters GetOrthographicProjectionParameters() const;
	void SetOrthographicProjectionParameters(OrthographicProjectionParameters parameters);

	// Move this later to private
	void SetPerspectiveProjection(const float fovy,const float aspect,const float near,const float far);
};

