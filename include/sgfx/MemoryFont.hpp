/*! \file */ //Copyright 2011-2018 Tyler Gilbert; All Rights Reserved

#ifndef SGFX_MEMORYFONT_HPP_
#define SGFX_MEMORYFONT_HPP_

#include "Font.hpp"

namespace sgfx {

class API_DEPRECATED_NO_REPLACEMENT MemoryFont;

class MemoryFont: public Font {
public:

	MemoryFont();

	MemoryFont(const void * ptr);
	void set_font_memory(const void * ptr);
	const void * font_memory() const { return m_font; }


	sg_size_t get_height() const;
	sg_size_t get_width() const;

protected:

	void draw_char_on_bitmap(const sg_font_char_t & ch, Bitmap & dest, const Point & point) const;
	int load_char(sg_font_char_t & ch, char c, bool ascii) const;
	int load_kerning(u16 first, u16 second) const;

private:

	int load_char_ptr(sg_font_char_t & ch, char c, bool ascii) const;
	int load_bitmap_ptr(const sg_font_char_t & ch) const;

	const void * m_font;
	mutable Bitmap m_canvas;

	u32 m_canvas_start;
	u32 m_canvas_size;

};

}

#endif /* SGFX_MEMORYFONT_HPP_ */
