//Copyright 2011-2018 Tyler Gilbert; All Rights Reserved

#include "sgfx.hpp"
#include "draw/Text.hpp"
using namespace draw;


Text::Text(const char * text){
	assign(text);
}

void Text::draw_to_scale(const DrawingScaledAttr & attr){
	sg_point_t top_left;
	int len;
	int h;
	Dim d = attr.dim();
	sg_point_t p = attr.point();
	const Font * font;

	if( text() ){

		font = resolve_font(d.height());

		if( font == 0 ){
			return;
		}
		h = font->get_height();
		len = font->calc_len(text());
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


		font->draw(text(), attr.bitmap(), top_left);

	}

}
