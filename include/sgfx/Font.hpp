
/*! \file */ //Copyright 2011-2016 Tyler Gilbert; All Rights Reserved

#ifndef SGFX_FONT_HPP_
#define SGFX_FONT_HPP_

#include <sgfx/sg_font_types.h>
#include "Bitmap.hpp"


namespace sgfx {

/*! \brief Font class
 *
 */
class Font {
public:

	Font();

	/*! \details Returns a string of the available character set */
	static const char * charset();

	enum {
		CHARSET_SIZE = 95
	};

	/*! \details This methods draws a character (\a c) on the font's
	 * bitmap and returns a pointer to the bitmap.
	 * @param c The character to draw
	 * @param ascii true if c is an ascii character or false the font is an icon font
	 *
	 * \return A pointer to a bitmap or zero if the character could not be loaded
	 */
	virtual const Bitmap & bitmap(char c, bool ascii = true) const = 0;

	//Attribute access methods
	int offset() const { return m_char.offset; }
	int yoffset() const { return m_char.yoffset; }


	/*! \details The maximum height of the font. */
	virtual sg_size_t get_h() const = 0;

	/*! \details Calulate the length (pixels on x-axis) of the specified string */
	int calc_len(const char * str) const;

	/*! \details Returns the number of characters in the font */
	int size() const { return m_hdr.num_chars; }

	/*! \details Set the spacing between letters within a word */
	inline void set_letter_spacing(sg_size_t spacing){ m_letter_spacing = spacing; }
	/*! \detials Returns the spacing of the letters within a word */
	inline sg_size_t letter_spacing() const { return m_letter_spacing; }

	/*! \details Set the number of pixels in a space between words */
	inline void set_space_size(int s){ m_space_size = s; }

	/*! \details Returns the number of pixels between words */
	inline int space_size() const { return m_space_size; }

	/*! \details Set the string pixels in the bitmap
	 *
	 * @param str The string to draw (or set)
	 * @param bitmap The bitmap to draw the string on
	 * @param point The top left corner to start drawing the string
	 * @return Zero on success
	 */
	int set_str(const char * str, Bitmap & bitmap, sg_point_t point) const;

	/*! \details Clear the string pixels in the bitmap
	 *
	 * @param str The string to draw (or clear)
	 * @param bitmap The bitmap to draw the string on
	 * @param point The top left corner to start drawing the string
	 * @return Zero on success
	 */
	int clear_str(const char * str, Bitmap & bitmap, sg_point_t point) const;


	int set_char(char c, Bitmap & bitmap, sg_point_t point) const;
	int clear_char(char c, Bitmap & bitmap, sg_point_t point) const;

protected:

	static int to_charset(char ascii);

	virtual int load_char(sg_font_char_t & ch, char c, bool ascii) const = 0;
	virtual int load_bitmap(const sg_font_char_t & ch) const = 0;
	virtual int load_kerning(u16 first, u16 second) const { return 0; }

	mutable int m_offset;
	mutable sg_font_char_t m_char;

	sg_size_t m_letter_spacing;
	int m_space_size;
	sg_font_hdr_t m_hdr;

private:

};

};



#endif /* SGFX_FONT_HPP_ */
