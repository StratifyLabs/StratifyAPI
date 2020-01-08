/*! \file */ //Copyright 2011-2018 Tyler Gilbert; All Rights Reserved

#include "sgfx.hpp"
#include "sys/Assets.hpp"
#include "draw/Icon.hpp"

#include "sys/Printer.hpp"

using namespace draw;

Icon::Icon(){
	m_rotation = RIGHT;
}



void Icon::draw_to_scale(const DrawingScaledAttr & attr){
	sg_point_t p = attr.point();

#if defined LEGACY_ICON
	if( &(this->icon()) == 0 ){
		return;
	}
#endif


	VectorPath vector_path = sys::Assets::find_vector_path(name());

	if( vector_path.is_valid() ){
		Bitmap bitmap(
					attr.area(),
					Bitmap::BitsPerPixel(attr.bitmap().bits_per_pixel())
					);
		bitmap.clear();
		bitmap << attr.bitmap().pen();

		sgfx::Vector::draw(
					bitmap,
					vector_path,
								 VectorMap().set_rotation(rotation())
								 .calculate_for_bitmap(bitmap)
					);

		//check for alignment values left/right/top/bottom
		if( is_align_top() ){
			p.y -= m_bounds.point.y;
		} else if( is_align_bottom() ){
			p.y += bitmap.height() - (m_bounds.point.y + m_bounds.area.height);
		}

		if( is_align_left() ){
			p.x -= m_bounds.point.x;
		} else if( is_align_right() ){
			p.y += bitmap.width() - (m_bounds.point.x + m_bounds.area.width);
		}

		//now draw on the bitmap
		attr.bitmap().draw_bitmap(p, bitmap);
	}
}
