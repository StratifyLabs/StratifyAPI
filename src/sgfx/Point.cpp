//Copyright 2011-2018 Tyler Gilbert; All Rights Reserved

#include <cmath>
#include "sgfx/Point.hpp"
using namespace sgfx;


Point Point::operator*(float f) const {
	float x, y;
	x = f * this->x();
	y = f * this->y();
	return Point(rintf(x), rintf(y));
}

Point Point::operator+(const Point & a) const{
	Point p(*this);
	api()->point_shift(&p.point(), a.point());
	return p;
}

Point Point::operator-(const Point & a) const {
	Point p(*this);
	api()->point_subtract(&p.point(), &a.point());
	return p;
}
