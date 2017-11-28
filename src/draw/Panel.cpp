/*! \file */ //Copyright 2011-2016 Tyler Gilbert; All Rights Reserved

#include <draw/Icon.hpp>
#include "sgfx.hpp"
#include "draw/Icon.hpp"
#include "draw/Panel.hpp"
using namespace draw;

Panel::Panel(){
	m_radius = 250;
	m_pen = Pen(1,1,true);
}


void Panel::draw_to_scale(const DrawingScaledAttr & attr){
	sg_vector_primitive_t objs[9];
	sg_vector_icon_t icon;
	VectorMap map;
	sg_size_t x_radius;
	sg_size_t y_radius;
	Pen p = attr.bitmap().pen();

	u32 radius;

	map.fill_region(attr.region());

	radius = m_radius * SG_MAX/4 / DrawingAttr::scale();

	if( attr.height() == attr.width() ){
		x_radius = radius;
		y_radius = radius;
	} else if( attr.height() > attr.width() ){
		x_radius = radius * attr.height() / attr.width();
		y_radius = radius;
	} else {
		y_radius = radius * attr.width() / attr.height();
		x_radius = radius;
	}

	icon.total = 9;
	icon.fill_total = 1;
	icon.primitives = objs;

	attr.bitmap().set_pen_flags(Pen::FLAG_IS_FILL);

	objs[0] = Vector::line(SG_LEFT+x_radius, SG_TOP, SG_RIGHT-x_radius, SG_TOP);
	objs[1] = Vector::line(SG_LEFT+x_radius, SG_BOT, SG_RIGHT-x_radius, SG_BOT);
	objs[2] = Vector::line(SG_LEFT, SG_TOP+y_radius, SG_LEFT, SG_BOT-y_radius);
	objs[3] = Vector::line(SG_RIGHT, SG_TOP+y_radius, SG_RIGHT, SG_BOT-y_radius);
	objs[4] = Vector::arc(SG_LEFT+x_radius, SG_TOP+y_radius, x_radius, y_radius, SG_TRIG_POINTS/2, SG_TRIG_POINTS*3/4);
	objs[5] = Vector::arc(SG_LEFT+x_radius, SG_BOT-y_radius, x_radius, y_radius, SG_TRIG_POINTS/4, SG_TRIG_POINTS/2);
	objs[6] = Vector::arc(SG_RIGHT-x_radius, SG_TOP+y_radius, x_radius, y_radius, SG_TRIG_POINTS*3/4, SG_TRIG_POINTS);
	objs[7] = Vector::arc(SG_RIGHT-x_radius, SG_BOT-y_radius, x_radius, y_radius, 0, SG_TRIG_POINTS/4);

	objs[8] = Vector::fill(0, 0);

	Vector::draw(attr.bitmap(), icon, map);

	attr.bitmap().set_pen(p);

}
