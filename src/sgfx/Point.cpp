//Copyright 2011-2016 Tyler Gilbert; All Rights Reserved


#include "sgfx/Point.hpp"
using namespace sgfx;


Point::Point(const sg_vector_primitive_t & a){
	m_value.x = 0;
	m_value.y = 0;
	rotate(a.rotation);
	shift(a.shift);
}


