//Copyright 2011-2016 Tyler Gilbert; All Rights Reserved

#include <cstdio>
#include <errno.h>
#include "sgfx/Font.hpp"

using namespace sgfx;

static const char m_ascii_charset[Font::CHARSET_SIZE+1] = " !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~";

const char * Font::charset(){
	return m_ascii_charset;
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

int Font::calc_len(const char * str) const {
	int l;
	l = 0;

	while( *str != 0 ){

		if( *str == ' ' ){
			l += space_size();
		} else {
			if( load_char(m_char, *str, true) == 0){
				l += m_char.xadvance;
			}
		}
		str++;
	}
	return l;
}

int Font::draw_char(char c, Bitmap & dest, sg_point_t point) const {

	if( load_char(m_char, c, true) < 0 ){
		return -1;
	}

	point.x += m_char.xoffset;
	point.y += m_char.yoffset;

	draw_char_on_bitmap(m_char, dest, point);

	//this needs to be char width not bitmap width
	return m_char.xadvance;
}

int Font::draw_str(const char * str, Bitmap & bitmap, sg_point_t point) const {
	char c;
	sg_size_t w;

	//draw characters on the bitmap
	while( (c = *(str++)) != 0){
		if( c == ' ' ){
			w = space_size();
		} else {
			draw_char(c, bitmap, point);
			w = m_char.xadvance;
		}

		//apply kerning
		w += load_kerning(c, *str);

		if( w < 0 ){
			return -1;
		}

		point.x += w;

	}
	return 0;
}

