#include "Shader.h"

#pragma region Public Methods
void Shader::CalculateVertxColors()
{
	colorA = calculatePhongReflection(NormalA, WorldPointA, calculateToCameraVector(WorldPointA));
	colorB = calculatePhongReflection(NormalB, WorldPointB, calculateToCameraVector(WorldPointB));
	colorC = calculatePhongReflection(NormalC, WorldPointC, calculateToCameraVector(WorldPointC));
}

const glm::vec4 Shader::GetColor() const
{
	switch (scene.GetSelectedShadingModel())
	{
	case Flat:
		return calculateColorFlat();
	case Phong:
		return calculateColorPhong();
	case Gouraud:
		return calculateColorGouraud();
	default:
		return glm::vec4(0);
	}
}
#pragma endregion

#pragma region const Calculations
const glm::vec4 Shader::calculateColorPhong() const
{
	//glm::vec4 out;
	//glm::vec4 ambientPart = calculateAmbientPart();
	//glm::vec4 diffusePart(0);
	//glm::vec4 spectralPart(0);

	//out = ambientPart + diffusePart + spectralPart;
	return glm::vec4(0);
}

const glm::vec4 Shader::calculateColorGouraud() const
{
	// Calculate the face normal by averaging the 3 normals
	//glm::vec3 faceNormal = (NormalA + NormalB + NormalC) / 3.0f;

	// Calculate (using phong reflection) the colors of each screenPoint
	glm::vec4 aColor = calculatePhongReflection(NormalA, WorldPointA, calculateToCameraVector(WorldPointA));
	glm::vec4 bColor = calculatePhongReflection(NormalB, WorldPointB, calculateToCameraVector(WorldPointB));
	glm::vec4 cColor = calculatePhongReflection(NormalC, WorldPointC, calculateToCameraVector(WorldPointC));

	// 
	return glm::vec4();
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
	float scalar = std::fmax(glm::dot(directionToLight, normal),0.0f);
	auto result = objectDiffuseColor * scalar * lightSource->GetColor();
	result.x = std::fmax(result.x, 0.0f);
	result.y = std::fmax(result.y, 0.0f);
	result.z = std::fmax(result.z, 0.0f);
	return result;
}


#pragma endregion

#pragma region Setters

void Shader::SetNormals(const glm::vec4 & a, const glm::vec4 & b, const glm::vec4 & c)
{
	NormalA = glm::normalize(a); 
	NormalB = glm::normalize(b); 
	NormalC = glm::normalize(c);
}


void Shader::SetWorldPoints(const glm::vec4 & worldPointA, const glm::vec4& worldPointB, const glm::vec4& worldPointC)
{
	WorldPointA = worldPointA;
	WorldPointB = worldPointB;
	WorldPointC = worldPointC;
}

void Shader::SetScreenPoints(const glm::vec3 & screenPointA, const glm::vec3 & screenPointB, const glm::vec3 & screenPointC)
{
	ScreenPointA = screenPointA;
	ScreenPointB = screenPointB;
	ScreenPointC = screenPointC;
}


#pragma endregion
