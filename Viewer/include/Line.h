#pragma once

#include "Point.h"

class Line
{
public:
	Line();
	Line(const Point& a, const Point& b);

	Point PointA;
	Point PointB;
	float GetSlope();

	void SetAToHaveSmallerXValue();
	void SetAToHaveSmallerYValue();
};
