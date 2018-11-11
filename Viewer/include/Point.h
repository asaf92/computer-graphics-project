#pragma once

class Point
{
public:
	double GetX() { return _x; }
	double GetY() { return _y; }

	Point(double x, double y) :_x(x), _y(y) {}

private:
	double _x;
	double _y;
};