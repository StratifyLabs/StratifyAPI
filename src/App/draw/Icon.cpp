/*! \file */ //Copyright 2011-2016 Tyler Gilbert; All Rights Reserved

#include "sgfx.hpp"
#include "draw/Icon.hpp"
using namespace draw;

Icon::Icon(){}

Icon::Icon(enum IconAttr::sys_gfx gfx) : m_attr(gfx){}

Icon::Icon(const IconAttr & gfx) : m_attr(gfx){}


const sg_icon_t * Icon::load_icon(enum IconAttr::sys_gfx gfx){
	/*
	asset_request_t req;
	const mg_icon_t * icon;
	if( gfx < (int)ASSET_MGFX_TOTAL ){
		req.ind = gfx;
		if( FSys::request(ASSET_REQUEST_MGFX, &req) < 0 ){
			return 0;
		}
		icon = (const mg_icon_t*)req.obj;
		return icon;
	}
	*/
	return 0;
}


void Icon::draw_to_scale(const DrawingScaledAttr & attr){
	sgfx::Gfx graphic;
	Bitmap * b = attr.b();
	sg_point_t p = attr.p();
	Dim d = attr.d();

	/*
	graphic.set_gfx(gfx(), d.w(), d.h(), &m_bounds);

	//check for alignment values left/right/top/bottom
	if( align_top() ){
		p.y -= bounds().top_left.y;
	} else if( align_bottom() ){
		p.y += icon.h() - bounds().bottom_right.y;
	}

	if( align_left() ){
		p.x -= bounds().top_left.x;
	} else if( align_right() ){
		p.y += icon.w() - bounds().bottom_right.x;
	}

	if( dark() ){
		b->clr_bitmap(&icon, p);
	} else {
		b->set_bitmap(&icon, p);
	}

	if( invert() ){
		b->invert(p, d);
	}
	*/

}
