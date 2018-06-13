//Copyright 2011-2018 Tyler Gilbert; All Rights Reserved

#include "ui/IconTab.hpp"
#include "draw.hpp"
using namespace ui;


IconTab::IconTab(){}

void IconTab::draw(const draw::DrawingAttr & attr){
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
