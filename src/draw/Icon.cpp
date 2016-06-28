/*! \file */ //Copyright 2011-2016 Tyler Gilbert; All Rights Reserved

#include "sgfx.hpp"
#include "draw/Icon.hpp"
using namespace draw;

Icon::Icon(){}


const sg_icon_t * IconAttr::get_system_icon(int icon){
	return IconSystem::get_icon(icon);
}


void Icon::draw_to_scale(const DrawingScaledAttr & attr){
	sg_point_t p = attr.p();
	sg_dim_t d = attr.d();

	Bitmap bitmap(d);
	bitmap.clear();

	GfxMap map(bitmap, this->attr().pen(), this->attr().rotation());
	Gfx::draw(bitmap, (this->attr().icon()), map.item(), &m_bounds);

	//check for alignment values left/right/top/bottom
	if( align_top() ){
		p.y -= m_bounds.top_left.y;
	} else if( align_bottom() ){
		p.y += bitmap.h() - m_bounds.bottom_right.y;
	}

	if( align_left() ){
		p.x -= m_bounds.top_left.x;
	} else if( align_right() ){
		p.y += bitmap.w() - m_bounds.bottom_right.x;
	}

	if( dark() ){
		attr.bitmap().clr_bitmap(bitmap, p);
	} else {
		attr.bitmap().set_bitmap(bitmap, p);
	}

	if( invert() ){
		attr.bitmap().invert(p, d);
	}

}
