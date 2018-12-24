#include "Point.h"

bool Point::IsInFrame(const float & width, const float & height)
{
	return X > 0 && Y > 0 && X < width && Y < height;
}
