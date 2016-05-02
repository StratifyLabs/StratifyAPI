/*! \file */ //Copyright 2011-2016 Tyler Gilbert; All Rights Reserved

#ifndef MBFONT_HPP_
#define MBFONT_HPP_

#include "MBitmap.hpp"
#include "MFont.hpp"

#include "mbfont.h"

namespace Draw {

/*! \brief Monochrome Bitmap Font class */
class MBFont : public MFont {
public:

	MBFont();


	/*! \brief Return a pointer to the character bitmap */
	/*! \details This methods draws a character (\a c) on the font's
	 * bitmap and returns a pointer to the bitmap.
	 * @param c The character to draw
	 * @param ascii true if c is an ascii character or false the font is an icon font
	 *
	 * \return A pointer to a bitmap or zero if the character could not be loaded
	 */
	virtual const MBitmap * bitmap(char c, bool ascii = true) const = 0;

	int offset() const { return _ch.offset; }
	int yoffset() const { return _ch.yoffset; }
	int len(const char * str) const;
	int size() const { return hdr.num_chars; }

	int set_str(const char * str, MBitmap * bitmap, mg_point_t point) const;
	int clr_str(const char * str, MBitmap * bitmap, mg_point_t point) const;
	int set_char(char c, MBitmap * bitmap, mg_point_t point) const;
	int clr_char(char c, MBitmap * bitmap, mg_point_t point) const;

	inline void set_letter_spacing(mg_size_t spacing){ _letter_spacing = spacing; }
	inline mg_size_t letter_spacing() const { return _letter_spacing; }

	inline void set_space_size(int s){ _space_size = s; }
	inline int space_size() const { return _space_size; }

protected:
	virtual int load_char(mbfont_char_t & ch, char c, bool ascii) const = 0;
	virtual int load_bitmap(const mbfont_char_t & ch) const = 0;
	virtual int load_kerning(u16 first, u16 second) const { return 0; }

	mutable MBitmap * _bitmap;
	mutable int _offset;
	mutable mbfont_char_t _ch;

	mg_size_t _letter_spacing;
	int _space_size;
	mbfont_hdr_t hdr;

private:

};

};

#endif /* MBFONT_HPP_ */
