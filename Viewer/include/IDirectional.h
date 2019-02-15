#pragma once
#include <glm/glm.hpp>

/*
* Interface for objects that have a changeable direction
*/
class IDirectional
{
public:
	/* Returns the direction vector */
	virtual glm::vec3 GetDirection() const = 0;

	/* Directly set a direction */
	virtual void SetDirection(const glm::vec3& direction) = 0;

	/* Change the direction horizontally. Positives number pan to the right */
	virtual void Pan(const float angle) = 0;

	/* Change the direction vertically. Positives number tilt upwards*/
	virtual void Tilt(const float angle) = 0;
};