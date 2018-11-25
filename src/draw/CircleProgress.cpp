//Copyright 2011-2018 Tyler Gilbert; All Rights Reserved

#include <cmath>
#include "sgfx.hpp"
#include "draw/CircleProgress.hpp"
using namespace draw;

CircleProgress::CircleProgress(){}

void CircleProgress::draw_to_scale(const DrawingScaledAttr & attr){
	//draw the progress bar on the bitmap with x, y at the top left corner
	sgfx::VectorMap map;

	sg_vector_icon_t icon;

	sg_vector_primitive_t primitives[3];

	int16_t end_angle;

	map.calculate_for_region(attr.region());

	Point end;

	end_angle = value() * SG_TRIG_POINTS / max() + SG_TRIG_POINTS*3/4;

	end.set(0, SG_TOP);
	end.rotate(value() * SG_TRIG_POINTS / max());

	primitives[0] = sgfx::Vector::line(0, 0, 0, SG_TOP);
	primitives[1] = sgfx::Vector::line(0, 0, end.x(),end.y());
	primitives[2] =  sgfx::Vector::arc(0, 0, SG_MAP_MAX, SG_MAP_MAX, SG_TRIG_POINTS*3/4, end_angle);
	icon.primitives = primitives;
	icon.total = 3;
	icon.fill_total = 0;

	Vector::draw(attr.bitmap(), icon, map);

}
