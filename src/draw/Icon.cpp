/*! \file */ //Copyright 2011-2016 Tyler Gilbert; All Rights Reserved

#include "sgfx.hpp"
#include "sys/Assets.hpp"
#include "draw/Icon.hpp"
using namespace draw;

Icon::Icon(){}


const sg_vector_icon_t * IconAttr::get_system_icon(int icon){
	return sys::Assets::get_vector_icon(icon);
}


void Icon::draw_to_scale(const DrawingScaledAttr & attr){
	sg_point_t p = attr.point();
	sg_dim_t d = attr.dim();

	if( &(this->icon()) == 0 ){
		return;
	}

	Bitmap bitmap(d);
	bitmap.clear();

	Pen tmp_pen = pen();

	tmp_pen.set_solid();

	bitmap.set_pen(tmp_pen);

	VectorMap map(bitmap, rotation());
	Vector::draw(bitmap, icon(), map.item(), &m_bounds);

	//check for alignment values left/right/top/bottom
	if( is_align_top() ){
		p.y -= m_bounds.top_left.y;
	} else if( is_align_bottom() ){
		p.y += bitmap.height() - m_bounds.bottom_right.y;
	}

	if( is_align_left() ){
		p.x -= m_bounds.top_left.x;
	} else if( is_align_right() ){
		p.y += bitmap.width() - m_bounds.bottom_right.x;
	}


	map.set_area(p, d, rotation());
	//Vector::draw(attr.bitmap(), icon(), map.item());

}
