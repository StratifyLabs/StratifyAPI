/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see LICENSE.md for rights.
#include "ux/Label.hpp"
#include "ux/draw/Rectangle.hpp"
#include "ux/draw/RichText.hpp"
#include "ux/EventLoop.hpp"

using namespace sgfx;
using namespace ux;

void Label::draw(const DrawingScaledAttributes & attributes){

	draw_base_properties(
				attributes.bitmap(),
				attributes.region(),
				theme()
				);

	Region region_inside_padding =
			calculate_region_inside_padding(attributes.region());

	draw::RichText()
			.set_icon_font_name(theme()->primary_icon_font_name())
			.set_text_font_name(theme()->primary_font_name())
			.set_value(value())
			.set_color(theme()->text_color())
			.set_alignment(alignment())
			.draw(
				attributes + region_inside_padding.point() + region_inside_padding.area()
				);


	apply_antialias_filter(attributes);

}
