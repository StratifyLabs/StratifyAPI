/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see LICENSE.md for rights.
//Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc

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

void SvgMemoryFont::draw_char_on_bitmap(const sg_font_char_t & ch, Bitmap & dest, const Point & point) const {


	//draw directly on the dest bitmap using Vector::draw_path()
}
