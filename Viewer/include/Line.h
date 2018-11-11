#pragma once

#include "Point.h"

class Line
{
public:
	Line();
	Line(Point a, Point b);

	double GetSlope() { return _slope; }
	double GetShift() { return _shift; }

private:
	double _slope;
	double _shift;
};