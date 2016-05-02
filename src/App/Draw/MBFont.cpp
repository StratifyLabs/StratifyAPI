//Copyright 2011-2016 Tyler Gilbert; All Rights Reserved


#include <errno.h>

#include "Draw/MBFont.hpp"
using namespace Draw;


MBFont::MBFont() {
	//max_byte_width_ = 0;
	//max_height_ = 0;
	_bitmap = 0;
	_offset = 0;
	_space_size = 8;
	_letter_spacing = 1;
}

int MBFont::len(const char * str) const {
	int l;
	l = 0;

	while( *str != 0 ){

		if( *str == ' ' ){
			l += space_size();
		} else {
			if( load_char(_ch, *str, true) < 0){
				return -1;
			}
			l += _ch.xadvance;
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

	point.x += _ch.xoffset;
	point.y += _ch.yoffset;

	bitmap->set_bitmap(cbp, point);

	//this needs to be char width not bitmap width
	return _ch.xadvance;
}

int MBFont::clr_char(char c, MBitmap * bitmap, mg_point_t point) const {
	const MBitmap * cbp;

	cbp = this->bitmap(c, true);
	if( cbp == 0 ){
		return -1;
	}

	point.x += _ch.xoffset;
	point.y += _ch.yoffset;

	bitmap->clr_bitmap(cbp, point);

	return _ch.xadvance;
}


int MBFont::clr_str(const char * str, MBitmap * bitmap, mg_point_t point) const {
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
			w = clr_char(c, bitmap, point);
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
			w = _ch.xadvance;
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

