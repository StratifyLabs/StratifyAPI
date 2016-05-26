/*! \file */ //Copyright 2011-2016 Tyler Gilbert; All Rights Reserved

#ifndef MBFONTMEMORY_HPP_
#define MBFONTMEMORY_HPP_


#include <draw/MBFont.hpp>

namespace draw {

/*! \brief Monochrome Bitmap Font class (stored in flash memory)
 *
 */
class MBFontMemory: public MBFont {
public:

	/*! \details Construct an object */
	MBFontMemory();

	/*! \details Construct an object pointing to the font. */
	MBFontMemory(const void * ptr);

	/*! \details Set the location of the font in memory
	 *
	 * @param ptr A pointer to the font (mbfont_hdr_t*)
	 */
	void set_font_memory(const void * ptr);

	/*! \details Returns a pointer to the font (\sa set())
	 *
	 */
	const void * font_memory() const { return m_font; }


	const MBitmap * bitmap(char c, bool ascii = true) const;

protected:
	int load_char(mbfont_char_t & ch, char c, bool ascii) const;
	int load_bitmap(const mbfont_char_t & ch) const;
	int load_kerning(u16 first, u16 second) const;

private:

	int load_char_ptr(mbfont_char_t & ch, char c, bool ascii) const;
	int load_bitmap_ptr(const mbfont_char_t & ch) const;

	const void * m_font;
	mutable MBitmap m_bitmap;

};

};

#endif /* MBFONTMEMORY_HPP_ */
