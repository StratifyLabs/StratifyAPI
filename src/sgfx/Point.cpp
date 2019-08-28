//Copyright 2011-2018 Tyler Gilbert; All Rights Reserved

#include <cmath>
#include "sgfx/Point.hpp"
using namespace sgfx;


Point Point::operator*(float f) const {
	float x, y;
	x = f * this->x();
	y = f * this->y();
	return Point(
				arg::XValue(rintf(x)),
				arg::YValue(rintf(y))
				);
}

Point & Point::operator+=(const arg::XValue x){
	*this = *this + x;
	return *this;
}

Point & Point::operator+=(const arg::YValue y){
	*this = *this + y;
	return *this;
}

Point & Point::operator-=(const arg::XValue x){
	*this = *this - x;
	return *this;
}

Point & Point::operator-=(const arg::YValue y){
	*this = *this - y;
	return *this;
}

Point Point::operator+(const Point & a) const{
	Point p(*this);
	api()->point_shift(&p.point(), a);
	return p;
}

Point Point::operator+(const arg::XValue x) const {
	Point p(*this);
	api()->point_shift(&p.point(), sg_point(x.argument(),0));
	return p;
}

Point Point::operator+(const arg::YValue y) const {
	Point p(*this);
	api()->point_shift(&p.point(), sg_point(0,y.argument()));
	return p;
}


Point Point::operator-(const Point & a) const {
	Point p(*this);
	api()->point_subtract(&p.point(), &a.point());
	return p;
}

Point Point::operator-(const arg::XValue x) const{
	Point p(*this);
	sg_point_t point;
	point.x = x.argument();
	point.y = 0;
	api()->point_subtract(&p.point(), &point);
	return p;
}

Point Point::operator-(const arg::YValue y) const {
	Point p(*this);
	sg_point_t point;
	point.x = 0;
	point.y = y.argument();
	api()->point_subtract(&p.point(), &point);
	return p;
}
