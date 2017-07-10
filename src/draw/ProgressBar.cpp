/*! \file */ //Copyright 2011-2016 Tyler Gilbert; All Rights Reserved

#include "sgfx.hpp"
#include "draw/ProgressBar.hpp"
using namespace draw;

ProgressBar::ProgressBar(){}

void ProgressBar::draw_to_scale(const DrawingScaledAttr & attr){
	//draw the progress bar on the bitmap with x, y at the top left corner
	int tmp;

	sg_point_t p = attr.p();
	Dim d = attr.d();

	Point middle;
	sg_int_t x_progress;
	sg_point_t end;


	end.x = p.x + d.w();
	end.y = p.y + d.h();

	tmp = (value() * (d.w()-4) + max()/2) / max();

	x_progress = p.x + 2 + tmp;

	//draw bar
	attr.bitmap().draw_rectangle(p, d);

	//clear un-progress section
	attr.bitmap().clear_rectangle(sg_point(x_progress, p.y + 2), sg_dim(end.x-3 - x_progress, d.h()-4));


}
