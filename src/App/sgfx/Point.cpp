//Copyright 2011-2016 Tyler Gilbert; All Rights Reserved


#include "sgfx/Point.hpp"
using namespace sgfx;


Point::Point(const sg_icon_primitive_t & a){
	d.x = 0;
	d.y = 0;
	rotate(a.rotation);
	shift(a.shift);
}


