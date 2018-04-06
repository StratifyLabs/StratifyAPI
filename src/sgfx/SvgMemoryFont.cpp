//Copyright 2011-2018 Tyler Gilbert; All Rights Reserved

#include "sgfx/SvgMemoryFont.hpp"

#include <cstdio>
#include <errno.h>

using namespace sgfx;

SvgMemoryFont::SvgMemoryFont() {

}

SvgMemoryFont::~SvgMemoryFont() {

}

int SvgMemoryFont::load_char(sg_font_char_t & ch, char c, bool ascii) const {
	return 0;
}

int SvgMemoryFont::load_kerning(u16 first, u16 second) const {
	return -1;
}

void SvgMemoryFont::draw_char_on_bitmap(const sg_font_char_t & ch, Bitmap & dest, sg_point_t point) const {


	//draw directly on the dest bitmap using Vector::draw_path()
}
