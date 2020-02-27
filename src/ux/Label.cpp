/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see LICENSE.md for rights.
#include "ux/Label.hpp"
#include "ux/draw/Rectangle.hpp"
#include "ux/draw/Text.hpp"
#include "ux/EventLoop.hpp"

using namespace sgfx;
using namespace ux;

void Label::draw_to_scale(const DrawingScaledAttributes & attributes){

	draw_base_properties(
				attributes.bitmap(),
				attributes.region(),
				theme()
				);

	Region region_inside_padding =
			calculate_region_inside_padding(attributes.area());

	//if the icon is available, draw it
	if( m_icon_name.is_empty() == false ){
		draw::Icon()
				.set_icon_font_name(theme()->primary_icon_font_name())
				.set_name(m_icon_name)
				.set_color(theme()->text_color())
				.set_alignment(alignment())
				.draw_to_scale(
					attributes + region_inside_padding.point() + region_inside_padding.area()
					);

	} else if( m_label.is_empty() == false ){
		//if the label is available, draw it
		draw::Text().set_string(m_label)
				.set_font_name(theme()->primary_font_name())
				.set_color(theme()->text_color())
				.set_alignment(alignment())
				.draw_to_scale(
					attributes + region_inside_padding.point() + region_inside_padding.area()
					);
	}

	apply_antialias_filter(attributes);

}
