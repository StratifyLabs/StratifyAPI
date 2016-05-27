//Copyright 2011-2016 Tyler Gilbert; All Rights Reserved

#include "var.hpp"
#include "sgfx.hpp"
#include "draw/TextBox.hpp"
using namespace draw;

TextBox::TextBox(){ set_font_size(16); m_scroll = 0; m_scroll_max = 0; }


TextBox::TextBox(const char * text){ assign(text); set_font_size(16); m_scroll = 0; m_scroll_max = 0; }


int TextBox::count_lines(sg_size_t w){
	return count_lines(text(), font_size(), font_bold(), w);
}

int TextBox::count_lines(const char * str, sg_size_t font, bool bold, sg_size_t w){
	Token tokens(str, " ");
	u32 i;
	String line;
	int len;
	int num_lines;
	i = 0;
	num_lines = 0;
	//App::select_system_font(font, bold);

	do {
		line.clear();
		build_line(i, line, tokens, len, w);
		num_lines++;
	} while( i < tokens.size() );
	return num_lines;
}


void TextBox::draw_to_scale(const DrawingScaledAttr & attr){
	Token tokens(text(), " ");
	String line;
	u32 i;
	sg_size_t w;
	Bitmap * b = attr.b();
	sg_point_t p = attr.p();
	sg_dim_t d = attr.d();
	sg_point_t start;

	sg_int_t line_y;
	sg_size_t font_height;
	sg_size_t num_lines;
	sg_size_t visible_lines;
	sg_size_t draw_line;
	int len;

	//draw the message and wrap the text
	//font_height = App::select_system_font(font_size(), font_bold());

	//draw each line of text; make sure it doesn't overlap
	w = d.w;
	line_y = 0;

	num_lines = count_lines(w);

	visible_lines = (d.h) / font_height;

	if( visible_lines >= num_lines ){
		m_scroll = 0;
		m_scroll_max = 0;
	}

	if( num_lines > visible_lines ){
		m_scroll_max = num_lines - visible_lines;
		if( m_scroll >= m_scroll_max ){
			m_scroll = m_scroll_max;
		}
	}

	draw_line = 0;
	i = 0;
	do {
		line.clear();
		build_line(i, line, tokens, len, w);
		if( (draw_line >= m_scroll) && (draw_line - m_scroll < visible_lines) ){

			start.y = p.y + line_y;
			if( align_left() ){
				start.x  = p.x;
			} else if( align_right() ){
				start.x  = p.x + w - len;
			} else {
				start.x = p.x + (w - len)/2;
			}
			//App::font()->set_str(line.c_str(), b, start);
			line_y += (font_height + font_height/10);
		}
		draw_line++;
	} while( i < tokens.size() );
}

void TextBox::build_line(u32 & i, String & line, Token & tokens, int & build_len, sg_size_t w){
	int len;
	int line_len;
	u32 j;
	u32 count = tokens.size();
	build_len = 0;

	for(j=i; j < count; j++){
		//line_len = App::font()->len(line.c_str());
		//len = App::font()->len(tokens.at(i));
		if( line_len + len <= w ){
			line << tokens.at(j);
			i++;
		}

		if( len > w ){
			//single word is too large to fit on one line
			line << tokens.at(j);
			i++;
			break;
		}

		//if( (line_len + len + App::font()->space_size() <= w) && (j < (count-1)) ){
		//	line << " ";
		//} else {
		//	break;
		//}
	}

	//build_len = App::font()->len(line.c_str());

}



