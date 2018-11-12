#include "Line.h"


Line::Line(Point* a, Point* b)
{
	// y = mx+n -> m= (y2-y1)/(x2-x1)
	_slope = (b->Y - a->Y) / (a->X - b->X);

	// y=mx+n -> n=y-mx
	_shift = a->Y - _slope * a->X;

	PointA = a;
	PointB = b;
}

Line::Line()
{
	_slope = 0;
	_shift = 0;
}

