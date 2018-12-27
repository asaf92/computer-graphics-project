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

const glm::vec4 Shader::CalculateColorFlat() const
{
	glm::vec4 color;
	// ignore the coordinates, just calculate the color of point A
	color = calculateAmbientPart();
	return color;
}

const glm::vec4 Shader::calculateAmbientPart() const
{
	return objectColor * ambientColor;
}

const glm::vec4 Shader::GetColor() const
{
	switch (selectedModel)
	{
	case Flat:
		return CalculateColorFlat();
	case Phong:
		return CalculateColorPhong();
	default:
		return glm::vec4(0);
	}
}

void Shader::SetWorldPoints(const Point & worldPointA, const Point & worldPointB, const Point & worldPointC)
{
	WorldPointA = worldPointA;
	WorldPointB = worldPointB;
	WorldPointC = worldPointC;
}

void Shader::SetScreenPoints(const Point & screenPointA, const Point & screenPointB, const Point & screenPointC)
{
	ScreenPointA = screenPointA;
	ScreenPointB = screenPointB;
	ScreenPointC = screenPointC;
}
