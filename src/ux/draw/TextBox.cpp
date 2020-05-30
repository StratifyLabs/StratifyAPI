/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see LICENSE.md for rights.
//Copyright 2011-2018 Tyler Gilbert; All Rights Reserved

#include "var.hpp"
#include "sgfx.hpp"
#include "sys/Assets.hpp"
#include "ux/draw/TextBox.hpp"

using namespace ux::draw;

TextBox::TextBox(){ m_scroll = 0; m_scroll_max = 0; }


int TextBox::count_lines(sg_size_t w){
	return count_lines(
				this->font(),
				string(),
				w
				);
}

int TextBox::count_lines(
		const Font * font,
		const var::String & string,
		sg_size_t w
		){

	StringList tokens = string.split(" ");
	u32 i;
	String line;
	int len;
	int num_lines;
	i = 0;
	num_lines = 0;

	if( font == nullptr ){
		return -1;
	}

	do {
		line.clear();
		build_line(font, i, line, tokens, len, w);
		num_lines++;
	} while( i < tokens.count() );

	return num_lines;
}


void TextBox::draw(
		const DrawingScaledAttributes & attr
		){

	u32 i;
	sg_size_t w;
	sg_point_t p = attr.point();
	sg_area_t d = attr.area();
	sg_point_t start;

	sg_int_t line_y;
	sg_size_t font_height;
	sg_size_t num_lines;
	sg_size_t visible_lines;
	sg_size_t draw_line;
	sg_size_t line_spacing;
	int len;
	const Font * font;

	//draw the message and wrap the text
	if( resolve_font(
				m_font_point_size == 0 ?
				d.height :
				m_font_point_size
				) == false ){
		return;
	}

	font = this->font();

	font_height = font->get_height();
	line_spacing = font_height/10;

	//draw each line of text; make sure it doesn't overlap
	w = d.width;
	line_y = 0;

	num_lines = count_lines(w);
	if( num_lines < 0 ){
		return;
	}

	visible_lines = (d.height) / (font_height + line_spacing);

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


	StringList line_list = string().split("\n");
	for(const String& input_line: line_list){
		StringList tokens = input_line.split(" ");
		String line;
		draw_line = 0;
		i = 0;
		do {
			line.clear();
			build_line(font, i, line, tokens, len, w);
			if( (draw_line >= m_scroll) && (draw_line - m_scroll < visible_lines) ){

				start.y = p.y + line_y;
				if( is_align_left() ){
					start.x  = p.x;
				} else if( is_align_right() ){
					start.x  = p.x + w - len;
				} else {
					start.x = p.x + (w - len)/2;
				}
				font->draw(line, attr.bitmap(), start);
				line_y += (font_height + line_spacing);
			}
			draw_line++;
		} while( i < tokens.count() );

	}

}

void TextBox::build_line(
		const Font * font,
		u32 & i,
		String & line,
		const StringList & tokens,
		int & build_len,
		sg_size_t w
		){
	int len;
	int line_len;
	u32 j;
	u32 count = tokens.count();
	build_len = 0;

	for(j=i; j < count; j++){
		line_len = font->calc_len(line.cstring());
		len = font->calc_len(tokens.at(i));
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

		if( (line_len + len + font->space_size() <= w) && (j < (count-1)) ){
			//there is room for another word -- add a space
			line.append(" ");
		} else {
			break;
		}

	}

	build_len = font->get_width(line);

}



