#pragma once
#include <memory>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include "MeshModel.h"
#include "ProjectionType.h"
#include "LookAtParameters.h"
#include "IMovable.h"
#include "IDirectional.h"
#include "IMoving.h"

/*
 * Camera class. This class takes care of all the camera transformations and manipulations.
 *
 * Implementation suggestion:
 * --------------------------
 * Make the Camera class be a subclass of MeshModel, so you can easily and elegantly render 
 * the cameras you have added to the scene.
 */

static float constexpr cameraMoveSpeed = 0.05f;

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

class Camera: public IMovable, public IDirectional, public IMoving
{
private:
	// Matrices
	glm::mat4x4 viewTransformation;
	glm::mat4x4 projectionTransformation;

	// Parameters
	LookAtParameters lookAtParameters;
	OrthographicProjectionParameters orthographicProjectionParameters;
	PerspectiveProjectionParameters perspectiveProjectionParameters;
	ProjectionType activeProjectionType;

	// Members
	glm::vec3 direction;

	// Projection functions
	void SetOrthographicProjection(float left, float right, float top, float bottom, float zNear, float zFar);
	void SetPerspectiveProjection(const float fovy,const float aspect,const float near,const float zFar);
	glm::mat4x4 CreateFrustum(float left, float right, float top, float bottom, float, float);

	// Library settings
	bool useLibraryProjectionMatrix = false;
	bool useLibraryViewMatrix = false;

public:
	Camera();
	Camera(const glm::vec3& eye, const glm::vec3& at, const glm::vec3& up);
	~Camera();

	// Library matrices overriding
	void SetUseLibraryProjectionMatrix(bool value) { useLibraryProjectionMatrix = value; }
	bool GetUseLibraryProjectionMatrix() { return useLibraryProjectionMatrix; }
	void SetUseLibraryViewMatrix(bool value) { useLibraryViewMatrix = value; }
	bool GetUseLibraryViewMatrix() { return useLibraryViewMatrix; }

	// Sets the viewTransformation matrix
	LookAtParameters GetLookAtParameters() const { return lookAtParameters; }
	const glm::vec3& GetCameraLocation() const { return lookAtParameters.at; }
	void SetCameraLookAt();
	void SetCameraLookAt(const glm::vec3& eye, const glm::vec3& at, const glm::vec3& up);
	void SetOrthographicProjection();
	void SetPerspectiveProjection();
	void RenderProjectionMatrix();

	const glm::mat4x4& GetViewMatrix() const { return viewTransformation; }
	const glm::mat4x4& GetProjectionMatrix() const { return projectionTransformation; }

	// Projection
	void SelectProjectionType(ProjectionType selection) { activeProjectionType = selection; }
	const ProjectionType GetProjectionType() const { return activeProjectionType; }

	float GetFoV()			{ return perspectiveProjectionParameters.fov; }
	float GetAspectRatio()	{ return perspectiveProjectionParameters.aspect; }
	float GetNear()			{ return perspectiveProjectionParameters.zNear; }
	float GetFar()			{ return perspectiveProjectionParameters.zFar; }

	// Projection parameters getters/setters
	const OrthographicProjectionParameters GetOrthographicProjectionParameters() const { return orthographicProjectionParameters; };
	void SetOrthographicProjectionParameters(OrthographicProjectionParameters parameters) { orthographicProjectionParameters = parameters; };
	const PerspectiveProjectionParameters GetPerspectiveProjectionParameters() const { return perspectiveProjectionParameters; };
	void SetPerspectiveProjectionParameters(PerspectiveProjectionParameters parameters) { perspectiveProjectionParameters = parameters; };

	// Inherited via IMovable
	virtual void Move(const glm::vec3 direction) override;

	// Inherited via IDirectional
	virtual glm::vec3 GetDirection() const override { return direction; };
	virtual void SetDirection(const glm::vec3 & direction) override { this->direction = direction; };
	virtual void Pan(const float angle) override;
	virtual void Tilt(const float angle) override;


	// Inherited via IMoving
	virtual void MoveForward() override;
	virtual void MoveBackwards() override;
	virtual void MoveLeft() override;
	virtual void MoveRight() override;

};

