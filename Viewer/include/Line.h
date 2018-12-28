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

	bool IsHorizontal() const { return round(PointA.Y) == round(PointB.Y); };
	bool IsVertical() const { return round(PointA.X) == round(PointB.X); };
};
