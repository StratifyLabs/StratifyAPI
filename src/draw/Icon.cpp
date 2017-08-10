/*! \file */ //Copyright 2011-2016 Tyler Gilbert; All Rights Reserved

#include "sgfx.hpp"
#include "draw/Icon.hpp"
using namespace draw;

Icon::Icon(){}


const sg_vector_icon_t * IconAttr::get_system_icon(int icon){
	return VectorIconSystem::get_icon(icon);
}


void Icon::draw_to_scale(const DrawingScaledAttr & attr){
	sg_point_t p = attr.p();
	sg_dim_t d = attr.d();
	Pen tmp_pen = this->pen();

	if( &(this->icon()) == 0 ){
		return;
	}

	Bitmap bitmap(d);
	bitmap.clear();

	VectorMap map(bitmap, tmp_pen, this->rotation());
	Vector::draw(bitmap, (this->icon()), map.item(), &m_bounds);

	//check for alignment values left/right/top/bottom
	if( align_top() ){
		p.y -= m_bounds.top_left.y;
	} else if( align_bottom() ){
		p.y += bitmap.height() - m_bounds.bottom_right.y;
	}

	if( align_left() ){
		p.x -= m_bounds.top_left.x;
	} else if( align_right() ){
		p.y += bitmap.width() - m_bounds.bottom_right.x;
	}

	attr.bitmap().draw_bitmap(p, bitmap);

}
