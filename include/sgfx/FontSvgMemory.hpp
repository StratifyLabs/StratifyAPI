/*! \file */ //Copyright 2011-2018 Tyler Gilbert; All Rights Reserved


#ifndef SGFX_FONTSVGMEMORY_HPP_
#define SGFX_FONTSVGMEMORY_HPP_

#include "FontSvg.hpp"

namespace sgfx {

/*! \brief Font SVG class
 *
 */
class FontSvgMemory : public FontSvg {
public:
	FontSvgMemory();
	virtual ~FontSvgMemory();

protected:
	void draw_char_on_bitmap(const sg_font_char_t & ch, Bitmap & dest, sg_point_t point) const;
	int load_char(sg_font_char_t & ch, char c, bool ascii) const;
	int load_kerning(u16 first, u16 second) const;
};

};

#endif /* SGFX_FONTSVGMEMORY_HPP_ */
