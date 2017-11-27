//Copyright 2011-2016 Tyler Gilbert; All Rights Reserved

#include <cmath>
#include "sgfx/Point.hpp"
using namespace sgfx;


Point Point::operator*(float f) const {
	Point p(*this);
	float x, y;
	x = f * p.x();
	y = f * p.y();
	p.set(rintf(x), rintf(y));
	return p;
}

Point Point::operator+(const sg_point_t & a) const{
	Point p(*this);
	sg_api()->point_shift(&p.m_value, a);
	return p;
}

Point Point::operator-(const sg_point_t & a) const {
	Point p(*this);
	sg_api()->point_subtract(&p.m_value, &a);
	return p;
}
