/*! \file */ //Copyright 2011-2016 Tyler Gilbert; All Rights Reserved

#include "sgfx.hpp"
#include "draw/Icon.hpp"
using namespace draw;

Icon::Icon(){}


const sg_icon_t * Icon::get_system_icon(int icon){
	return GfxSystem::get_icon(icon);
}


void Icon::draw_to_scale(const DrawingScaledAttr & attr){

	Bitmap * b = attr.b();
	sg_point_t p = attr.p();
	sg_dim_t d = attr.d();
	sg_bounds_t bounds;

	Bitmap bitmap(d);

	bitmap.clear();

	GfxMap map(bitmap, this->attr().o_thickness_fill(), this->attr().rotation());

	Gfx::draw(bitmap, (this->attr().icon()), map.item(), &bounds);

	//check for alignment values left/right/top/bottom
	if( align_top() ){
		p.y -= bounds.top_left.y;
	} else if( align_bottom() ){
		p.y += bitmap.h() - bounds.bottom_right.y;
	}

	if( align_left() ){
		p.x -= bounds.top_left.x;
	} else if( align_right() ){
		p.y += bitmap.w() - bounds.bottom_right.x;
	}

	if( dark() ){
		b->clr_bitmap(bitmap, p);
	} else {
		b->set_bitmap(bitmap, p);
	}

	if( invert() ){
		b->invert(p, d);
	}

}
