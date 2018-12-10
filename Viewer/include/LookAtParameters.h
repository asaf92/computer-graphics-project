#pragma once
#include <glm/gtc/type_ptr.hpp> // For glm

struct LookAtParameters
{
	glm::vec3& eye = glm::vec3(0,0,-1);
	glm::vec3& at = glm::vec3(0,0,1);
	glm::vec3& up = glm::vec3(0,1,0);
};