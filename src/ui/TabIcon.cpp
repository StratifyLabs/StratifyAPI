//Copyright 2011-2016 Tyler Gilbert; All Rights Reserved

#include "ui/TabIcon.hpp"
#include "draw.hpp"
using namespace ui;


TabIcon::TabIcon(){}

void TabIcon::draw(const draw::DrawingAttr & attr){
	drawing_point_t p;
	drawing_dim_t d;
	DrawingAttr icon_draw_attr;

	d = attr.dim();
	p = attr.point();
	Tab::calc_square(p, d);

	icon_draw_attr = attr;
	icon_draw_attr.set_point(p);
	icon_draw_attr.set_dim(d);

	icon().draw(icon_draw_attr);
}
