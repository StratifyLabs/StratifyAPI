//Copyright 2011-2016 Tyler Gilbert; All Rights Reserved

#include "draw.hpp"
#include "ui/Tab.hpp"
using namespace ui;

Tab::Tab(){ set_element(0); }


void Tab::calc_square(drawing_point_t & p, drawing_dim_t & dim){
	if( dim.w > dim.h ){
		p.x = p.x + (dim.w - dim.h) / 2;
		dim.w = dim.h;
	} else {
		p.y = p.y + (dim.h - dim.w) / 2;
		dim.h = dim.w;
	}
}
