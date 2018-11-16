#pragma once

#include "Point.h"

class Line
{
public:
	Line();
	Line(const Point& a, const Point& b);

	Point PointA;
	Point PointB;
	double GetSlope() { return _slope; }
	double GetShift() { return _shift; }

private:
	double _slope;
	double _shift;
};