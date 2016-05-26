//Copyright 2011-2016 Tyler Gilbert; All Rights Reserved


#include <errno.h>

#include "Draw/MBFont.hpp"
using namespace Draw;


MBFont::MBFont() {
	//max_byte_width_ = 0;
	//max_height_ = 0;
	m_bitmap = 0;
	m_offset = 0;
	m_space_size = 8;
	m_letter_spacing = 1;
}

int MBFont::len(const char * str) const {
	int l;
	l = 0;

	while( *str != 0 ){

		if( *str == ' ' ){
			l += space_size();
		} else {
			if( load_char(m_char, *str, true) < 0){
				return -1;
			}
			l += m_char.xadvance;
		}
		str++;
	}
	return l;
}

int MBFont::set_char(char c, MBitmap * bitmap, mg_point_t point) const {
	const MBitmap * cbp;

	cbp = this->bitmap(c, true);
	if( cbp == 0 ){
		return -1;
	}

	point.x += m_char.xoffset;
	point.y += m_char.yoffset;

	bitmap->set_bitmap(cbp, point);

	//this needs to be char width not bitmap width
	return m_char.xadvance;
}

int MBFont::clear_char(char c, MBitmap * bitmap, mg_point_t point) const {
	const MBitmap * cbp;

	cbp = this->bitmap(c, true);
	if( cbp == 0 ){
		return -1;
	}

	point.x += m_char.xoffset;
	point.y += m_char.yoffset;

	bitmap->clear_bitmap(cbp, point);

	return m_char.xadvance;
}


int MBFont::clear_str(const char * str, MBitmap * bitmap, mg_point_t point) const {
	char c;
	mg_size_t w;
	if( bitmap == 0 ){
		return -1;
	}
	//draw characters on the bitmap
	while( (c = *(str++)) != 0){
		if( c == ' ' ){
			w = space_size();
		} else {
			w = clear_char(c, bitmap, point);
		}
		if( w < 0 ){
			return -1;
		}

		//apply kerning
		w += load_kerning(c, str[1]);
		point.x += w;
	}
	return 0;
}

int MBFont::set_str(const char * str, MBitmap * bitmap, mg_point_t point) const {
	char c;
	mg_size_t w;
	if( bitmap == 0 ){
		return -1;
	}
	//draw characters on the bitmap
	while( (c = *(str++)) != 0){
		if( c == ' ' ){
			w = space_size();
		} else {
			set_char(c, bitmap, point);
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

