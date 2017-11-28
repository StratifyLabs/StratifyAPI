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

	if( &(this->icon()) == 0 ){
		return;
	}

	Bitmap bitmap(attr.dim());
	bitmap.clear();
	bitmap.set_pen( pen() );

	VectorMap map(bitmap, rotation());
	Vector::draw(bitmap, icon(), map, &m_bounds);

	//check for alignment values left/right/top/bottom
	if( is_align_top() ){
		p.y -= m_bounds.point.y;
	} else if( is_align_bottom() ){
		p.y += bitmap.height() - (m_bounds.point.y + m_bounds.dim.height);
	}

	if( is_align_left() ){
		p.x -= m_bounds.point.x;
	} else if( is_align_right() ){
		p.y += bitmap.width() - (m_bounds.point.x + m_bounds.dim.width);
	}

	attr.bitmap().draw_bitmap(p, bitmap);
}
