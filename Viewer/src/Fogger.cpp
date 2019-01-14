#include "Fogger.h"

float Fogger::getFogFactor(float zValue)
{
	float nominator = finish - zValue;
	float denominator = finish - start;
	float out = nominator / denominator;
	out = std::min(out, 1.0f);
	out = std::max(out, 0.0f);
	return out;
}

void Fogger::AddFog()
{
	glm::vec3 color;
	float zValue;
	float fogFactor;
	for (int x = 0; x < viewportWidth; x++)
	{
		for (int y = 0; y < viewportHeight; y++)
		{
			color.r =  colorBuffer[INDEX(viewportWidth, x, y, 0)];
			color.g =  colorBuffer[INDEX(viewportWidth, x, y, 1)];
			color.b =  colorBuffer[INDEX(viewportWidth, x, y, 2)];
			zValue = zBuffer[INDEX(viewportWidth, x, y, 0)];
			zValue += 2.0f; // Magic number to solve negative zValues issue
			fogFactor = getFogFactor(zValue);
			color *= fogFactor;
			color += (1 - fogFactor) * fogColor;
			colorBuffer[INDEX(viewportWidth, x, y, 0)] = color.x;
			colorBuffer[INDEX(viewportWidth, x, y, 1)] = color.y;
			colorBuffer[INDEX(viewportWidth, x, y, 2)] = color.z;
		}
	}
}
