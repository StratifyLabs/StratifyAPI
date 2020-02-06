/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see LICENSE.md for rights.
//Copyright 2011-2018 Tyler Gilbert; All Rights Reserved

#include "sgfx.hpp"
#include "draw/Text.hpp"
#include "sys/Assets.hpp"
#include "sys/Printer.hpp"

using namespace draw;


Text::Text(const var::String & text){
	m_string = text;
	m_font = nullptr;
	m_font_point_size = 0;
	m_font_style = FontInfo::REGULAR;
}

const Font * Text::resolve_font(sg_size_t h) const{
	const FontInfo * info = sys::Assets::find_font(
				sgfx::Font::PointSize(h),
				sgfx::Font::Style(m_font_style)
				);
	if( info ){ return info->font(); }
	return 0;
}


void Text::draw_to_scale(const DrawingScaledAttr & attr){
	sg_point_t top_left;
	int len;
	int h;
	Area d = attr.area();
	sg_point_t p = attr.point();
	const Font * font;

	if( !string().is_empty() ){

		if( this->font() == nullptr ){
			font = resolve_font(m_font_point_size == 0 ? d.height() : m_font_point_size);
			if( font == 0 ){
				return;
			}
		} else {
			font = this->font();
		}

		if( font == nullptr ){
			return;
		}

		h = font->get_height();
		len = font->calculate_length(string());
		top_left.y = p.y;
		if( is_align_left() ){
			top_left.x = p.x;
		} else if( is_align_right() ){
			top_left.x = p.x + d.width() - len;
		} else {
			//center by default
			top_left.x = p.x + d.width()/2 - len/2;
		}

		if( is_align_top() ){
			//top
			top_left.y = p.y;
		} else if( is_align_bottom() ){
			//bottom
			top_left.y = p.y + d.height() - h;
		} else {
			//center
			top_left.y = p.y + d.height()/2 - h/2;
		}

		font->draw(
					string(),
					attr.bitmap() << Pen().set_color(color()),
					top_left);
	}

}
