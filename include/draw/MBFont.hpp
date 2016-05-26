/*! \file */ //Copyright 2011-2016 Tyler Gilbert; All Rights Reserved

#ifndef MBFONT_HPP_
#define MBFONT_HPP_

#include <draw/mbfont.h>
#include <draw/MBitmap.hpp>
#include <draw/MFont.hpp>

namespace draw {

/*! \brief Monochrome Bitmap Font class */
class MBFont : public MFont {
public:

	MBFont();

	/*! \details This methods draws a character (\a c) on the font's
	 * bitmap and returns a pointer to the bitmap.
	 * @param c The character to draw
	 * @param ascii true if c is an ascii character or false the font is an icon font
	 *
	 * \return A pointer to a bitmap or zero if the character could not be loaded
	 */
	virtual const MBitmap * bitmap(char c, bool ascii = true) const = 0;

	//Attribute access methods
	int offset() const { return m_char.offset; }
	int yoffset() const { return m_char.yoffset; }
	int len(const char * str) const;
	int size() const { return m_hdr.num_chars; }
	inline void set_letter_spacing(mg_size_t spacing){ m_letter_spacing = spacing; }
	inline mg_size_t letter_spacing() const { return m_letter_spacing; }
	inline void set_space_size(int s){ m_space_size = s; }
	inline int space_size() const { return m_space_size; }

	int set_str(const char * str, MBitmap * bitmap, mg_point_t point) const;
	int clear_str(const char * str, MBitmap * bitmap, mg_point_t point) const;
	int set_char(char c, MBitmap * bitmap, mg_point_t point) const;
	int clear_char(char c, MBitmap * bitmap, mg_point_t point) const;

protected:
	virtual int load_char(mbfont_char_t & ch, char c, bool ascii) const = 0;
	virtual int load_bitmap(const mbfont_char_t & ch) const = 0;
	virtual int load_kerning(u16 first, u16 second) const { return 0; }

	mutable MBitmap * m_bitmap;
	mutable int m_offset;
	mutable mbfont_char_t m_char;

	mg_size_t m_letter_spacing;
	int m_space_size;
	mbfont_hdr_t m_hdr;

private:

};

};

#endif /* MBFONT_HPP_ */
