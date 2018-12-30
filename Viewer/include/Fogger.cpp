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
	float r, g, b;
	glm::vec3 color;
	float zValue;
	float fogFactor;
	for (int x = 0; x < viewportWidth; x++)
	{
		for (int y = 0; y < viewportHeight; y++)
		{
			r =  colorBuffer[INDEX(viewportWidth, x, y, 0)];
			g =  colorBuffer[INDEX(viewportWidth, x, y, 1)];
			b =  colorBuffer[INDEX(viewportWidth, x, y, 2)];
			zValue = zBuffer[INDEX(viewportWidth, x, y, 0)];
			fogFactor = getFogFactor(zValue);
			color = glm::vec4(r, g, b,0) * fogFactor;
		}
	}
}
