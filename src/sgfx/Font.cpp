//Copyright 2011-2018 Tyler Gilbert; All Rights Reserved

#include <cstdio>
#include <errno.h>
#include "var/Token.hpp"
#include "sys/File.hpp"
#include "sgfx/Font.hpp"

using namespace sgfx;

FontInfo::FontInfo(u8 point_size, u8 style, const Font * font){
	m_point_size = point_size;
	m_style = style;
	m_font = font;
}

FontInfo::FontInfo(const var::ConstString & path){
	m_path = path;

	var::Tokenizer tokens(sys::File::name(path), "-.");

	if( tokens.count() != 4 ){
		m_point_size = 0;
		printf("Font name is not valid\n");
	} else {

		m_font = 0;
		m_name = tokens.at(0);
		m_point_size = tokens.at(2).to_integer();
		var::String style = tokens.at(1);
		style.to_lower();

		m_style = ANY;
		if( style == "t" ){ m_style = THIN; }
		if( style == "ti" ){ m_style = THIN_ITALIC; }
		if( style == "el" ){ m_style = EXTRA_LIGHT; }
		if( style == "eli" ){ m_style = EXTRA_LIGHT_ITALIC; }
		if( style == "l" ){ m_style = LIGHT; }
		if( style == "li" ){ m_style = LIGHT_ITALIC; }
		if( style == "r" ){ m_style = REGULAR; }
		if( style == "ri" ){ m_style = REGULAR_ITALIC; }
		if( style == "m" ){ m_style = MEDIUM; }
		if( style == "mi" ){ m_style = MEDIUM_ITALIC; }
		if( style == "sb" ){ m_style = SEMI_BOLD; }
		if( style == "sbi" ){ m_style = SEMI_BOLD_ITALIC; }
		if( style == "b" ){ m_style = BOLD; }
		if( style == "bi" ){ m_style = BOLD_ITALIC; }
		if( style == "eb" ){ m_style = EXTRA_BOLD; }
		if( style == "ebi" ){ m_style = EXTRA_BOLD_ITALIC; }
		printf("Font is %s %d %d\n", m_name.cstring(), m_point_size, m_style);
	}
}


int FontInfo::ascending_point_size(const void * a, const void * b){
	const FontInfo * info_a = (const FontInfo *)a;
	const FontInfo * info_b = (const FontInfo *)b;
	if( info_a->point_size() < info_b->point_size() ){ return -1; }
	if( info_b->point_size() < info_a->point_size() ){ return 1; }
	return 0;
}

int FontInfo::ascending_style(const void * a, const void * b){
	const FontInfo * info_a = (const FontInfo *)a;
	const FontInfo * info_b = (const FontInfo *)b;
	if( info_a->style() < info_b->style() ){ return -1; }
	if( info_b->style() < info_a->style() ){ return 1; }
	return 0;
}

const var::ConstString Font::m_ascii_character_set = " !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~";

const var::ConstString & Font::ascii_character_set(){
	return m_ascii_character_set;
}

int Font::to_charset(char ascii){
	if( (ascii < ' ') || (ascii > '~') ){
		return -1;
	}
	return (int)(ascii - ' ' - 1);
}

Font::Font() {
	//m_bitmap = 0;
	m_offset = 0;
	m_space_size = 8;
	m_letter_spacing = 1;
}

int Font::calculate_length(const var::ConstString & str) const {
	u32 length = 0;
	const char * s = str.cstring();
	while( *s != 0 ){

		if( *s == ' ' ){
			length += space_size();
		} else {
			if( load_char(m_char, *s, true) == 0){
				length += m_char.advance_x;
			}
		}
		s++;
	}
	return length;
}

int Font::draw(char c, Bitmap & dest, const Point & point) const {

	if( load_char(m_char, c, true) < 0 ){
		return -1;
	}

	Point p = point + Point(m_char.offset_x, m_char.offset_y);

	draw_char_on_bitmap(m_char, dest, p);

	return m_char.advance_x;
}

int Font::draw(const var::ConstString & const_string, Bitmap & bitmap, const Point & point) const {
	char c;
	sg_size_t w;

	//draw characters on the bitmap
	Point p(point);
	u32 i = 0;
	while( (c = const_string.at(i++)) != 0){
		if( c == ' ' ){
			w = space_size();
		} else {
			draw(c, bitmap, p);
			w = m_char.advance_x;
		}

		//apply kerning
		w += load_kerning(c, const_string.at(i));

		if( w < 0 ){
			return -1;
		}



		p += Point(w,0);

	}
	return 0;
}

