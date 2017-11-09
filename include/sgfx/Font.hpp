
/*! \file */ //Copyright 2011-2016 Tyler Gilbert; All Rights Reserved

#ifndef SGFX_FONT_HPP_
#define SGFX_FONT_HPP_

#include <sapi/sg_font_types.h>
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

	/*! \details Accesses the bitmap used to draw characters. */
	virtual const Bitmap & bitmap() const = 0;


	/*! \details Returns the maximum height of any character in the font. */
	virtual sg_size_t get_height() const = 0;

	/*! \details Returns the maximum width of any character in the font. */
	virtual sg_size_t get_width() const = 0;

	//Attribute access methods
	int offset() const { return m_char.offset; }
	int yoffset() const { return m_char.yoffset; }


	/*! \details Calculates the length (pixels on x-axis) of the specified string. */
	int calc_len(const char * str) const;

	/*! \details Returns the number of characters in the font. */
	int size() const { return m_hdr.num_chars; }

	/*! \details Sets the spacing between letters within a word. */
	void set_letter_spacing(sg_size_t spacing){ m_letter_spacing = spacing; }

	/*! \details Returns the spacing of the letters within a word. */
	sg_size_t letter_spacing() const { return m_letter_spacing; }

	/*! \details Sets the number of pixels in a space between words. */
	void set_space_size(int s){ m_space_size = s; }

	/*! \details Returns the number of pixels between words. */
	int space_size() const { return m_space_size; }

	/*! \details Draws the string on the specified bitmap.
	 *
	 * @param str The string to draw (or set)
	 * @param dest The bitmap to draw the string on
	 * @param point The top left corner to start drawing the string
	 * @return Zero on success
	 */
	int draw_str(const char * str, Bitmap & dest, sg_point_t point) const;

	/*! \details Draws a character on the bitmap.
	 *
	 * @param c The character to draw
	 * @param dest The destination bitmap
	 * @param point The top-left corner where to draw on \a dest
	 * @return Zero on success (-1 if character fails to load)
	 */
	int draw_char(char c, Bitmap & dest, sg_point_t point) const;

	const sg_font_char_t & character() const { return m_char; }

	/*! \details Accesses the number of characters in the font. */
	u16 num_chars() const { return m_hdr.num_chars; }
	/*! \details Accesses the number of bits per pixel in the font. */
	u16 bits_per_pixel() const { return m_hdr.bits_per_pixel; }
	/*! \details Accesses the number of kerning pairs in the font. */
	u16 kerning_pairs() const { return m_hdr.kerning_pairs; }

protected:

	static int to_charset(char ascii);

	virtual int load_char(sg_font_char_t & ch, char c, bool ascii) const = 0;
	virtual int load_kerning(u16 first, u16 second) const { return 0; }

	mutable int m_offset;
	mutable sg_font_char_t m_char;

	sg_size_t m_letter_spacing;
	int m_space_size;
	sg_font_header_t m_hdr;

private:

};

};



#endif /* SGFX_FONT_HPP_ */
