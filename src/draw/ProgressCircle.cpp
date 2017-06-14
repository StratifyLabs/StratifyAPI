//Copyright 2011-2016 Tyler Gilbert; All Rights Reserved

#include <cmath>
#include "sgfx.hpp"
#include "draw/ProgressCircle.hpp"
using namespace draw;

ProgressCircle::ProgressCircle(){}

void ProgressCircle::draw_to_scale(const DrawingScaledAttr & attr){
	//draw the progress bar on the bitmap with x, y at the top left corner
	sgfx::VectorMap map;

	sg_point_t p = attr.p();
	Dim d = attr.d();
	sg_vector_icon_t icon;

	sg_vector_primitive_t primitives[3];

	int16_t end_angle;

	map.set_shift(p.x + d.w()/2, p.y + d.h()/2);
	map.set_dim(d.w()*2, d.h()*2);

	Point end;

	end_angle = value() * SG_TRIG_POINTS / max() + SG_TRIG_POINTS*3/4;

	end.set(0, SG_TOP);
	end.rotate(value() * SG_TRIG_POINTS / max());

	primitives[0] = sgfx::Vector::line(0, 0, 0, SG_TOP);
	primitives[1] = sgfx::Vector::line(0, 0, end.x(),end.y());
	primitives[2] =  sgfx::Vector::arc(0, 0, SG_MAX/2, SG_MAX/2, SG_TRIG_POINTS*3/4, end_angle);
	icon.primitives = primitives;
	icon.total = 3;
	icon.fill_total = 0;

	Vector::draw(attr.bitmap(), icon, map);

}
