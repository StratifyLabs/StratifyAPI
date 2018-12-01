//Copyright 2011-2018 Tyler Gilbert; All Rights Reserved

#include <cstdio>
#include <errno.h>
#include "sgfx/Font.hpp"

using namespace sgfx;

const var::ConstString Font::m_ascii_character_set = " !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~";

const var::ConstString & Font::ascii_character_set(){
	return m_ascii_character_set;
}

int Font::to_charset(char ascii){
	if( (ascii < ' ') || (ascii > '~') ){
		return -1;
	}
	return (int)(ascii - ' ');
}

Font::Font() {
	//m_bitmap = 0;
	m_offset = 0;
	m_space_size = 8;
	m_letter_spacing = 1;
}

int Font::calculate_length(const var::ConstString & str) const {
	int l;
	l = 0;

	const char * s = str.str();

	while( *s != 0 ){

		if( *s == ' ' ){
			l += space_size();
		} else {
			if( load_char(m_char, *s, true) == 0){
				l += m_char.advance_x;
			}
		}
		s++;
	}
	return l;
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

