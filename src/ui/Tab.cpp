//Copyright 2011-2016 Tyler Gilbert; All Rights Reserved

#include "draw.hpp"
#include "ui/Tab.hpp"
using namespace ui;

Tab::Tab(){ set_element(0); }


void Tab::calc_square(drawing_point_t & p, drawing_dim_t & dim){
	if( dim.width > dim.height ){
		p.x = p.x + (dim.width - dim.height) / 2;
		dim.width = dim.height;
	} else {
		p.y = p.y + (dim.height - dim.width) / 2;
		dim.height = dim.width;
	}
}
