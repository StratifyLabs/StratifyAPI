/*! \file */ //Copyright 2011-2018 Tyler Gilbert; All Rights Reserved

#include "sgfx.hpp"
#include "sys/Assets.hpp"
#include "ux/Icon.hpp"

#include "sys/Printer.hpp"

using namespace ux;

Icon::Icon(){
	m_rotation = rotation_right;
}

void Icon::draw_to_scale(const DrawingScaledAttributes & attr){
	sg_point_t p = attr.point();

	VectorPath vector_path = sys::Assets::find_vector_path(icon());

	if( vector_path.is_valid() ){
		attr.bitmap() << Pen().set_color( m_color );

		Bitmap bitmap(
					attr.area(),
					Bitmap::BitsPerPixel(attr.bitmap().bits_per_pixel())
					);
		bitmap.clear();
		bitmap << attr.bitmap().pen();

		sgfx::Vector::draw(
					attr.bitmap(),
					vector_path,
					VectorMap().set_rotation(m_rotation)
					.calculate_for_region(attr.region())
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
