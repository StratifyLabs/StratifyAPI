/*! \file */ //Copyright 2011-2016 Tyler Gilbert; All Rights Reserved

#ifndef FONT8_HPP_
#define FONT8_HPP_

#include "Font8Util.hpp"
#include "MBFont.hpp"

namespace draw {

/*! \brief Bitmap Fonts (used with LCDs)
 * \details This class is for using fonts on LCDs.  It has
 * 95 characters per font with a size up to 8x8.
 *
 */
class Font8 : public MBFont {
public:
	Font8(const font8_t * f = 0);

	/*! \brief Load the font from disk.
	 * \details This function saves a font to disk.
	 */
	int load(const char * path);
	MBitmap bitmap(char c) const;
	inline int h() const { return m_font->h; }
	inline int w() const { return m_font->w; }

private:
	const font8_t * m_font;
};

};

#endif /* FONT8_HPP_ */
