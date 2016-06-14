//Copyright 2011-2016 Tyler Gilbert; All Rights Reserved

#include <cmath>
#include "sgfx.hpp"
#include "draw/ProgressCircle.hpp"
using namespace draw;

ProgressCircle::ProgressCircle(){}

void ProgressCircle::draw_to_scale(const DrawingScaledAttr & attr){
	//draw the progress bar on the bitmap with x, y at the top left corner
	sg_map_t map;

	sg_point_t p = attr.p();
	Dim d = attr.d();
	sg_icon_t icon;

	sg_icon_primitive_t elements[3];

	int16_t end_angle;

	map.shift.x = p.x + d.w()/2;
	map.shift.y = p.y + d.h()/2;
	map.size.w = d.w()*2;
	map.size.h = d.h()*2;
	map.rotation = 0;
	map.o_thickness_fill = 2;
	map.op = SG_OP_SET;
	Point end;

	end_angle = value() * SG_TRIG_POINTS / max() + SG_TRIG_POINTS*3/4;

	end.set(0, SG_TOP);
	end.rotate(value() * SG_TRIG_POINTS / max());

	elements[0] = sgfx::Gfx::line(0, 0, 0, SG_TOP);
	elements[1] = sgfx::Gfx::line(0, 0, end.x(),end.y());
	elements[2] =  sgfx::Gfx::arc(0, 0, SG_MAX/2, SG_MAX/2, SG_TRIG_POINTS*3/4, end_angle);
	icon.elements = elements;
	icon.total = 3;
	icon.fill_total = 0;

	Gfx::draw(attr.bitmap(), icon, map);

}
