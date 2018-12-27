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
	color = calculatePhongReflection(NormalA,WorldPointA,calculateToCameraVector(calculateFaceWorldCenter()));
	return color;
}

const glm::vec4 Shader::calculatePhongReflection(const glm::vec4& normal,const glm::vec4& worldPoint, const glm::vec4& toCamera) const
{
	glm::vec4 ambientPart = calculateAmbientPart();
	glm::vec4 lightSum(0);
	for each (LightSource* light in scene.GetLightsVector())
	{
		glm::vec4 diffusePartSum = calculateDiffusePart (normal,worldPoint,light);
		glm::vec4 spectralPartSum = calculateSpectralPart();
		lightSum += diffusePartSum + spectralPartSum;
	}
	return ambientPart + lightSum;
}

const glm::vec4 Shader::calculateAmbientPart() const
{
	return objectColor * ambientColor;
}

// Calculate the diffuse part for a single light source
const glm::vec4 Shader::calculateDiffusePart(const glm::vec4& normal, const glm::vec4& worldPoint, const LightSource* lightSource) const
{
	glm::vec4 directionToLight;
	const std::vector<LightSource*>& lightSources = scene.GetLightsVector();
	directionToLight = lightSource->GetDirectionToLightSource(worldPoint);
	auto result = objectDiffuseColor * (directionToLight * normal);
	result.x = std::fmax(result.x, 0.0f);
	result.y = std::fmax(result.y, 0.0f);
	result.z = std::fmax(result.z, 0.0f);
	return result;
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
