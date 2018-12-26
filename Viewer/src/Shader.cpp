#include "Shader.h"

const glm::vec4 Shader::CalculateColorPhong(const glm::vec4 & color) const
{
	glm::vec4 out;
	glm::vec4 ambientPart = calculateAmbientPart(color);
	glm::vec4 diffusePart(0);
	glm::vec4 spectralPart(0);

	out = ambientPart + diffusePart + spectralPart;
	return out;
}

const glm::vec4 Shader::calculateAmbientPart(const glm::vec4 & color) const
{
	return color * ambientColor;
}

const glm::vec4 Shader::CalculateColor(const glm::vec4& color) const
{
	switch (selectedModel)
	{
	case Phong:
		return CalculateColorPhong(color);
	default:
		return glm::vec4(0);
	}
}