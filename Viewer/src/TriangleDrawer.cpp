#include "TriangleDrawer.h"
#pragma region Constructors
TriangleDrawer::TriangleDrawer(Shader & _shader, PixelPlacer & _pixelPlacer, int _viewportWidth, int _viewportHeight) :
	shader(_shader),
	pixelPlacer(_pixelPlacer),
	viewportWidth(_viewportWidth),
	viewportHeight(_viewportHeight)
{}

#pragma endregion Constructors

#pragma region PublicMethods
void TriangleDrawer::SetUnscaledPoints(const Point & pointA, const Point & pointB, const Point & pointC)
{
	unscaledPointA = Utils::Vec4FromPoint(pointA);
	unscaledPointB = Utils::Vec4FromPoint(pointB);
	unscaledPointC = Utils::Vec4FromPoint(pointC);
}


void TriangleDrawer::DrawTriangle()
{
	// Convert points to screenPoints and update shader
	screenPointA = Utils::ScreenVec3FromWorldPoint(unscaledPointA, viewportWidth, viewportHeight);
	screenPointB = Utils::ScreenVec3FromWorldPoint(unscaledPointB, viewportWidth, viewportHeight);
	screenPointC = Utils::ScreenVec3FromWorldPoint(unscaledPointC, viewportWidth, viewportHeight);
	shader.SetScreenPoints(screenPointA, screenPointB, screenPointC);
	// Check if screenPoints are not in frame, return if necessary
	if (!allPointsAreInFrame())
	{
		return;
	}

	// Set z, minX,minY,maxX,maxY and triangleHit bool
	bool triangleHit = false;
	XYBorders borders = minMax();
	BycentricCoordinates bycentricCoordinates;
	const int minX = (int)floor(borders.minX);
	const int minY = (int)floor(borders.minY);
	const int maxX = (int)ceil(borders.maxX);
	const int maxY = (int)ceil(borders.maxY);
	const float z = (screenPointA.z + screenPointB.z + screenPointC.z) / 3.0f;
	// If A, B, C share X or Y value
	//if((int)round(screenPointA.x) == (int)round(screenPointB.x)  ||
	//	(int)round(screenPointA.x) == (int)round(screenPointC.x) ||
	//	(int)round(screenPointB.x) == (int)round(screenPointC.x))
	
	for (int x = minX; x < maxX; x++)
	{
		triangleHit = false;
		for (int y = minY; y < maxY; y++)
		{
			bycentricCoordinates = getBycentricCoordinates(x,y);
			if (pointInTriangle(bycentricCoordinates))
			{
				shader.SetCoords(x, y);
				pixelPlacer.PutPixel(x, y, shader.GetColor(), z);
				triangleHit = true;
			}
			else if (triangleHit) continue;  // This is an optimization. After drawing some pixels in the triangle and finding that the next y
		}								     // value is not inside the triangle, we can assert that the rest of the Y values will also 
	}									     // not be in the triangle
}

void TriangleDrawer::SetViewport(int viewportWidth, int viewportHeight)
{
	this->viewportWidth = viewportWidth;
	this->viewportHeight = viewportHeight;
}
#pragma endregion PublicMethods

#pragma region PrivateMethods

XYBorders TriangleDrawer::minMax() const
{
	XYBorders out;
	out.minX = (float)std::min(screenPointA.x, std::min(screenPointB.x, screenPointC.x));
	out.minY = (float)std::min(screenPointA.y, std::min(screenPointB.y, screenPointC.y));
	out.maxX = (float)std::max(screenPointA.x, std::max(screenPointB.x, screenPointC.x));
	out.maxY = (float)std::max(screenPointA.y, std::max(screenPointB.y, screenPointC.y));

	return out;
}

bool TriangleDrawer::pointInTriangle(int _x, int _y)
{
	BycentricCoordinates bycentricCoords = getBycentricCoordinates(_x, _y);
	// Round to zero
	if (abs(bycentricCoords.B) < EPSILON) bycentricCoords.B = 0.0f;
	if (abs(bycentricCoords.C) < EPSILON) bycentricCoords.C = 0.0f;
	float w1w2 = bycentricCoords.B + bycentricCoords.C - 1.0f;
	return bycentricCoords.B >= 0.0 && bycentricCoords.C >= 0.0f && w1w2 < 0.0f || abs(w1w2) < EPSILON;
}

bool TriangleDrawer::pointInTriangle(const BycentricCoordinates& bycentricCoords) const
{
	float w1 = bycentricCoords.B;
	float w2 = bycentricCoords.C;
	float w3 = bycentricCoords.A;
	// Round to zero
	if (abs(w1) < EPSILON) w1 = 0.0f;
	if (abs(w2) < EPSILON) w2 = 0.0f;
	float w1w2 = w1 + w2 - 1.0f;
	return w1 >= 0.0 && w2 >= 0.0f && w1w2 < 0.0f || abs(w1w2) < EPSILON;
}

BycentricCoordinates TriangleDrawer::getBycentricCoordinates(int _x, int _y)
{
	BycentricCoordinates out;
	float Px = (float)_x;
	float Py = (float)_y;
	float CYminAY = (screenPointC.y - screenPointA.y);
	float CXminAX = (screenPointC.x - screenPointA.x);
	float BYminAY = (screenPointB.y - screenPointA.y);
	float PYminAY = (Py - screenPointA.y);
	out.B = ((screenPointA.x * CYminAY) + (PYminAY * CXminAX) - (Px * CYminAY)) / ((BYminAY*CXminAX) - ((screenPointB.x - screenPointA.x) *CYminAY));
	out.C = (PYminAY - (out.B * BYminAY)) / CYminAY;
	out.A = 1 - out.C - out.B;

	return out;
}

bool TriangleDrawer::allPointsAreInFrame()
{
	return screenPointA.x >= 0 && screenPointA.x < viewportWidth &&
		   screenPointB.x >= 0 && screenPointB.x < viewportWidth && 
		   screenPointC.x >= 0 && screenPointC.x < viewportWidth && 
		   screenPointA.y >= 0 && screenPointA.y < viewportHeight &&
		   screenPointB.y >= 0 && screenPointB.y < viewportHeight &&
		   screenPointC.y >= 0 && screenPointC.y < viewportHeight;
}

//float TriangleDrawer::calcWOneValue(const glm::vec3& A, const glm::vec3& B, const glm::vec3& C, int _x, int _y)
//{
//	//if (A.Y == C.Y) return 0.0f;
//	float Px = (float)_x;
//	float Py = (float)_y;
//	float CYminAY = (C.y - A.y);
//	float CXminAX = (C.x - A.x);
//	float BYminAY = (B.y - A.y);
//	float BXminAX = (B.x - A.x);
//	return ()/();
//}
//
//float TriangleDrawer::calcWTwoValue(const glm::vec3 & A, const glm::vec3& B, const glm::vec3& C, int _y, float w1)
//{
//	//if (C.Y == A.Y) return 0.0f;
//	float y = (float)_y;
//	float nominator = (y - A.y - (w1 * (B.y - A.y)));
//	float denominator = (C.y - A.y);
//	return nominator / denominator;
//}

#pragma endregion PrivateMethods