#include "Line.h"


Line::Line(const Point& a, const Point& b)
{
	PointA = a;
	PointB = b;
}

Line::Line()
{
	
}

float Line::GetSlope() const
{
	return (PointB.Y - PointA.Y) / (PointA.X - PointB.X);
}

void Line::SetAToHaveSmallerXValue()
{
	if (PointA.X < PointB.X) return;
	// simply swap the two if needed
	Point tmp = PointA;
	PointA = PointB;
	PointB = tmp;
}

void Line::SetAToHaveSmallerYValue()
{
	if (PointA.Y < PointB.Y) return;
	// simply swap the two if needed
	Point tmp = PointA;
	PointA = PointB;
	PointB = tmp;
}