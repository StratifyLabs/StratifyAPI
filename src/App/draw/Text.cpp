//Copyright 2011-2016 Tyler Gilbert; All Rights Reserved

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
	Dim d = attr.d();
	sg_point_t p = attr.p();
	Font * font;

	if( text() ){
		if( font_size() == 0 ){
			h = d.h();
		} else {
			h = font_size();
		}
		font = FontSystem::get_font(h, font_bold());
		if( font == 0 ){
			return;
		}
		h = font->get_h();
		len = font->calc_len(text());
		top_left.y = p.y;
		if( align_left() ){
			top_left.x = p.x;
		} else if( align_right() ){
			top_left.x = p.x + d.w() - len;
		} else {
			//center by default
			top_left.x = p.x + d.w()/2 - len/2;
		}

		if( align_top() ){
			//top
			top_left.y = p.y;
		} else if( align_bottom() ){
			//bottom
			top_left.y = p.y + d.h() - h;
		} else {
			//center
			top_left.y = p.y + d.h()/2 - h/2;
		}


		if( dark() ){
			font->clear_str(text(), *attr.b(), top_left);
		} else {
			font->set_str(text(), *attr.b(), top_left);
		}
	}

}
