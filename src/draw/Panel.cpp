/*! \file */ //Copyright 2011-2016 Tyler Gilbert; All Rights Reserved

#include <draw/Icon.hpp>
#include "sgfx.hpp"
#include "draw/Icon.hpp"
#include "draw/Panel.hpp"
using namespace draw;

Panel::Panel(){}


void Panel::draw_to_scale(const DrawingScaledAttr & attr){
	Icon gfx;
	sg_size_t w, h;
	DrawingScaledAttr scaled_attr = attr;
	sg_point_t p;
	sg_dim_t d;


	w = attr.d().w;
	h = attr.d().h;
	gfx.set_dark(false);

	/*
	if( h == w ){
		gfx.attr().set(IconAttr::PANEL, 0, 0);
		gfx.draw_to_scale(attr);
	} else {
		gfx.attr().set(IconAttr::PANEL_HALF, 0, 0);
		if( h > w ){
			//draw half panel on the top and the bottom -- fill the middle
			gfx.attr().set_rotation(IconAttr::UP);
			scaled_attr.set_dim(w,w);
			gfx.draw_to_scale(scaled_attr);

			scaled_attr.set_y(attr.y() + h - w);
			gfx.attr().set_rotation(IconAttr::DOWN);
			gfx.draw_to_scale(scaled_attr);
			p.x = attr.x() + gfx.sg_attr().top_left.x;
			p.y = attr.y() + (w+1)/2;
			d.w = gfx.sg_attr().bottom_right.x - gfx.sg_attr().top_left.x + 1;
			d.h = h - w;
			attr.b()->set(p, d, 0xFF);
		} else { //w > h
			//draw half panel on the left and the right -- fill the middle
			gfx.attr().set_rotation(IconAttr::LEFT);
			scaled_attr.set_dim(h,h);
			gfx.draw_to_scale(scaled_attr);

			scaled_attr.set_x(attr.x() + w - h);
			gfx.attr().set_rotation(IconAttr::RIGHT);

			gfx.draw_to_scale(scaled_attr);
			p.x = attr.x()+(h+1)/2;
			p.y = attr.y() + gfx.sg_attr().top_left.y;
			d.w = w - h;
			d.h = gfx.sg_attr().bottom_right.y - gfx.sg_attr().top_left.y + 1;
			attr.b()->set(p, d, 0xFF);
		}
	}
	*/
}
