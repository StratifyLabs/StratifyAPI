/*! \file */ //Copyright 2011-2018 Tyler Gilbert; All Rights Reserved


#ifndef SGFX_FONTSVG_HPP_
#define SGFX_FONTSVG_HPP_

#include "Font.hpp"

namespace sgfx {

/*! \brief Font SVG class
 *
 */
class FontSvg {
public:
	FontSvg();
	virtual ~FontSvg();

	virtual const Bitmap & bitmap() const;
	virtual sg_size_t get_height() const;
	virtual sg_size_t get_width() const;

protected:
	int load_char(sg_font_char_t & ch, char c, bool ascii) const;
	int load_kerning(u16 first, u16 second) const;
};

};

#endif /* SGFX_FONTSVG_HPP_ */
