/*! \file */ //Copyright 2011-2018 Tyler Gilbert; All Rights Reserved


#ifndef SGFX_SVGFONT_HPP_
#define SGFX_SVGFONT_HPP_

#include "Font.hpp"

namespace sgfx {

/*! \brief Stratify Vector Graphics Font
 * \details The SvgFont class is used to draw
 * fonts using vector graphic data rather then
 * bitmap data.
 *
 */
class SvgFont : public Font {
public:
    SvgFont();
    virtual ~SvgFont();

	virtual sg_size_t get_height() const { return m_height; }
	virtual sg_size_t get_width() const { return m_width; }

	void set_height(sg_size_t height);

private:
	sg_size_t m_height;
	sg_size_t m_width;
};

}

#endif /* SGFX_SVGFONT_HPP_ */
