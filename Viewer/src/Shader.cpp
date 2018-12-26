#include "Shader.h"

const glm::vec4 Shader::CalculateColorPhong() const
{
	glm::vec4 out;
	glm::vec4 ambientPart = calculateAmbientPart();
	glm::vec4 diffusePart(0);
	glm::vec4 spectralPart(0);

	out = ambientPart + diffusePart + spectralPart;
	return out;
}

const glm::vec4 Shader::calculateAmbientPart() const
{
	return objectColor * ambientColor;
}

const glm::vec4 Shader::CalculateColor() const
{
	switch (selectedModel)
	{
	case Phong:
		return CalculateColorPhong();
	default:
		return glm::vec4(0);
	}
}