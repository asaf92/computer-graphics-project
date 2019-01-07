#include "Shader.h"

#pragma region Public Methods
void Shader::CalculateVertexColors()
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
	case Gouraud:
		return calculateColorGouraud();
	case Phong:
		return calculateColorPhong();
	default:
		return glm::vec4(0);
	}
}
#pragma endregion

#pragma region const Calculations
const glm::vec4 Shader::calculateColorPhong() const
{
	glm::vec4 interpolatedNormal;
	interpolatedNormal  = bycentricCoordinates.A * NormalA;
	interpolatedNormal += bycentricCoordinates.B * NormalB;
	interpolatedNormal += bycentricCoordinates.C * NormalC;
	interpolatedNormal = glm::normalize(interpolatedNormal);
	
	glm::vec4 interpolatedWorldPoint;
	interpolatedWorldPoint  = bycentricCoordinates.A * WorldPointA;
	interpolatedWorldPoint += bycentricCoordinates.B * WorldPointB;
	interpolatedWorldPoint += bycentricCoordinates.C * WorldPointC;
	
	glm::vec4 toCamera = calculateToCameraVector(interpolatedWorldPoint);

	return calculatePhongReflection(interpolatedNormal, interpolatedWorldPoint, toCamera);
}

const glm::vec4 Shader::calculateColorGouraud() const
{
	glm::vec4 out; 
	out  = bycentricCoordinates.A * colorA;
	out += bycentricCoordinates.B * colorB;
	out += bycentricCoordinates.C * colorC;
	return out;
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
		glm::vec4 spectralPartSum = calculateSpecularPart(normal,worldPoint,toCamera,light);
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
	directionToLight = glm::normalize(lightSource->GetDirectionToLightSource(worldPoint));
	float scalar = std::fmax(glm::dot(directionToLight, normal),0.0f);
	auto result = objectDiffuseColor * scalar * lightSource->GetColor();
	result.x = std::fmax(result.x, 0.0f);
	result.y = std::fmax(result.y, 0.0f);
	result.z = std::fmax(result.z, 0.0f);
	return result;
}

const glm::vec4 Shader::calculateSpecularPart(const glm::vec4& normal, const glm::vec4& worldPoint, const glm::vec4& toCamera, const LightSource* lightSource) const
{
	glm::vec4 directionToLight = glm::normalize(-lightSource->GetDirectionToLightSource(worldPoint));
	glm::vec4 reflection = glm::normalize(2.0f * (glm::dot(directionToLight, normal)) * normal - directionToLight);
	float scalar = glm::dot(reflection, toCamera);
	scalar = pow(scalar, shininess);
	auto result = objectSpecularColor * scalar * lightSource->GetColor();
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
