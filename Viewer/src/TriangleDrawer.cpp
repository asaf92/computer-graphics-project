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

	// Check if screenPoints are not in frame, return if necessary
	if (!allPointsAreInFrame())
	{
		return;
	}

	// Set z, minX,minY,maxX,maxY and triangleHit bool
	bool triangleHit = false;
	XYBorders borders = minMax();
	const int minX = (int)floor(borders.minX);
	const int minY = (int)floor(borders.minY);
	const int maxX = (int)ceil(borders.maxX);
	const int maxY = (int)ceil(borders.maxY);
	const float z = (screenPointA.z + screenPointB.z + screenPointC.z) / 3.0f;
	// If A, B, C share X or Y value
		// drawStraightLine (don't stop the function)
	
	for (int x = minX; x < maxX; x++)
	{
		triangleHit = false;
		for (int y = minY; y < maxY; y++)
		{
			if (pointInTriangle(x, y))
			{
				shader.SetCoords(x, y);
				pixelPlacer.PutPixel(x, y, shader.GetColor(), z);
				triangleHit = true;
			}
			else if (triangleHit) break;  // This is an optimization. After drawing some pixels in the triangle and finding that the next y
		}								  // value is not inside the triangle, we can assert that the rest of the Y values will also 
	}									  // not be in the triangle
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
	float Px = (float)_x;
	float Py = (float)_y;
	float CYminAY = (screenPointC.y - screenPointA.y);
	float CXminAX = (screenPointC.x - screenPointA.x);
	float BYminAY = (screenPointB.y - screenPointA.y);
	float PXminAX = (Py - screenPointA.y);
	float w1 = ((screenPointA.x * CYminAY) + (PXminAX * CXminAX) - (Px * CYminAY)) / ((BYminAY*CXminAX) - ((screenPointB.x - screenPointA.x) *CYminAY));
	float w2 = (PXminAX - (w1 * BYminAY)) / CYminAY;
	return w1 >= 0.0 && w2 >= 0.0f && (w1 + w2) <= 1.0f;
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

void TriangleDrawer::drawStraightLine(glm::vec3 pointA, glm::vec4 pointB)
{

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