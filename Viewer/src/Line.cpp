#include "Line.h"


Line::Line(const Point& a, const Point& b)
{
	PointA.X = a.X;
	PointA.Y = a.Y;
	PointB.X = b.X;
	PointB.Y = b.Y;

	// y = mx+n -> m= (y2-y1)/(x2-x1)
	_slope = (b.Y - a.Y) / (a.X - b.X);

	// y=mx+n -> n=y-mx
	_shift = a.Y - _slope * a.X;

}

Line::Line()
{
	_slope = 0;
	_shift = 0;
}
