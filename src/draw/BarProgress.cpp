/*! \file */ //Copyright 2011-2018 Tyler Gilbert; All Rights Reserved

#include "sgfx.hpp"
#include "draw/BarProgress.hpp"
using namespace draw;

BarProgress::BarProgress(){}

void BarProgress::draw_to_scale(const DrawingScaledAttr & attr){
	//draw the progress bar on the bitmap with x, y at the top left corner
	int tmp;
	Pen pen;
	sg_int_t x_progress;
	sg_size_t thickness;
	sg_size_t thickness2;

	pen = attr.bitmap().pen();

	thickness = scaled_border_thickness();
	if( thickness*3 > attr.height() ){
		thickness = attr.height()/3;
	}

	thickness2 = thickness*2;

	tmp = (value() * (attr.width()-thickness2) + max()/2) / max();

	x_progress = tmp;

	//draw bar
	attr.bitmap().set_pen_thickness(1);
	attr.bitmap().draw_rectangle(attr.region());

	//clear un-progress section
	attr.bitmap().set_pen_flags(Pen::IS_ERASE);
	attr.bitmap().draw_rectangle(Region(
											  Point(attr.x() + thickness + x_progress, attr.y() + thickness),
											  Area(attr.width() - thickness2 - x_progress, attr.height()-thickness2))
										  );
	attr.bitmap().set_pen(pen);

}
