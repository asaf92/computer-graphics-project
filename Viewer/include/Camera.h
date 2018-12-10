#pragma once
#include <memory>
#include <glm/glm.hpp>
#include "MeshModel.h"
#include "ProjectionType.h"
#include "LookAtParameters.h"

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
	LookAtParameters lookAtParameters;
	glm::mat4x4 viewTransformation;
	glm::mat4x4 projectionTransformation;
	OrthographicProjectionParameters orthographicProjectionParameters;
	PerspectiveProjectionParameters perspectiveProjectionParameters;
	ProjectionType activeProjectionType;
	float zoom;
	void SetOrthographicProjection(float left, float right, float top, float bottom, float zNear, float zFar);
	glm::mat4x4 CreateFrustum(float left, float right, float top, float bottom, float, float);

public:
	Camera();
	Camera(const glm::vec3& eye, const glm::vec3& at, const glm::vec3& up);
	~Camera();

	// Sets the viewTransformation matrix
	LookAtParameters GetLookAtParameters() const { return lookAtParameters; }
	void SetCameraLookAt(const glm::vec3& eye, const glm::vec3& at, const glm::vec3& up);
	void SetOrthographicProjection();
	void SetPerspectiveProjection();
	void RenderProjectionMatrix();
	void SetZoom(const float zoom) { this->zoom = zoom; };
	float GetZoom() const { return zoom; }

	const glm::mat4x4& GetViewMatrix() const { return viewTransformation; }
	const glm::mat4x4& GetProjectionMatrix() const { return projectionTransformation; }

	// Projection
	void SelectProjectionType(ProjectionType selection) { activeProjectionType = selection; }
	const ProjectionType GetProjectionType() const { return activeProjectionType; }

	float GetFoV()			{ return perspectiveProjectionParameters.fov; }
	float GetAspectRatio()	{ return perspectiveProjectionParameters.aspect; }
	float GetNear()			{ return perspectiveProjectionParameters.zNear; }
	float GetFar()			{ return perspectiveProjectionParameters.zFar; }

	const OrthographicProjectionParameters GetOrthographicProjectionParameters() const;
	void SetOrthographicProjectionParameters(OrthographicProjectionParameters parameters);
	const PerspectiveProjectionParameters GetPerspectiveProjectionParameters() const { return perspectiveProjectionParameters; };
	void SetPerspectiveProjectionParameters(PerspectiveProjectionParameters parameters) { perspectiveProjectionParameters = parameters; };

	// Move this later to private
	void SetPerspectiveProjection(const float fovy,const float aspect,const float near,const float far);
};

