#include "Line.h"


Line::Line(Point a, Point b)
{
	// y = mx+n -> m= (y2-y1)/(x2-x1)
	_slope = (b.GetY() - a.GetY()) / (a.GetX() - b.GetX());

	// y=mx+n -> n=y-mx
	_shift = a.GetY() - _slope * a.GetX();
}

Line::Line()
{
	_slope = 0;
	_shift = 0;
}

