//Copyright 2011-2018 Tyler Gilbert; All Rights Reserved

#include "sgfx.hpp"
#include "draw/Text.hpp"
using namespace draw;


Text::Text(const var::ConstString & text){
	string() = text;
}

void Text::draw_to_scale(const DrawingScaledAttr & attr){
	sg_point_t top_left;
	int len;
	int h;
	Area d = attr.area();
	sg_point_t p = attr.point();
	const Font * font;

	if( !string().is_empty() ){

		if( this->font() == 0 ){
			font = resolve_font(d.height());
			if( font == 0 ){ return; }
		} else {
			font = this->font();
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


		font->draw(string(), attr.bitmap(), top_left);

	}

}
