#include "Shader.h"

const glm::vec4 Shader::calculateColorPhong() const
{
	//glm::vec4 out;
	//glm::vec4 ambientPart = calculateAmbientPart();
	//glm::vec4 diffusePart(0);
	//glm::vec4 spectralPart(0);

	//out = ambientPart + diffusePart + spectralPart;
	return glm::vec4(0);
}

const glm::vec4 Shader::calculateColorFlat() const
{
	glm::vec4 color;
	// ignore the coordinates, just calculate the color of point A
	color = calculatePhongReflection(NormalA,CameraWorldPoint);
	return color;
}

const glm::vec4 Shader::calculatePhongReflection(const glm::vec4& normal, const glm::vec4& toViewer) const
{
	glm::vec4 ambientPart = calculateAmbientPart ();
	glm::vec4 diffusePart = calculateDiffusePart ();
	glm::vec4 spectralPart= calculateSpectralPart();
	glm::vec4 lightSum = diffusePart + spectralPart; // For each light!!!
	return ambientPart + lightSum;
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
		return calculateColorFlat();
	case Phong:
		return calculateColorPhong();
	default:
		return glm::vec4(0);
	}
}

void Shader::SetWorldPoints(const Point & worldPointA, const Point & worldPointB, const Point & worldPointC)
{
	WorldPointA = Utils::Vec4FromPoint(worldPointA);
	WorldPointB = Utils::Vec4FromPoint(worldPointB);
	WorldPointC = Utils::Vec4FromPoint(worldPointC);
}

void Shader::SetScreenPoints(const Point & screenPointA, const Point & screenPointB, const Point & screenPointC)
{
	ScreenPointA = screenPointA;
	ScreenPointB = screenPointB;
	ScreenPointC = screenPointC;
}
