#pragma once

#include "Point.h"

class Line
{
public:
	Line();
	Line(const Point& a, const Point& b);

	Point PointA;
	Point PointB;
	float GetSlope() const;

	void SetAToHaveSmallerXValue();
	void SetAToHaveSmallerYValue();

	bool IsHorizontal() const { return PointA.Y == PointB.Y; };
	bool IsVertical() const { return PointA.X == PointB.X; };
};
