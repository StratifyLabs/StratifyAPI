/*! \file */ //Copyright 2011-2018 Tyler Gilbert; All Rights Reserved


#ifndef SGFX_FONTSVG_HPP_
#define SGFX_FONTSVG_HPP_

#include "Font.hpp"

namespace sgfx {

class FontSvg : public Font {
public:
	FontSvg();
	virtual ~FontSvg();

	virtual sg_size_t get_height() const { return m_height; }
	virtual sg_size_t get_width() const { return m_width; }

	void set_height(sg_size_t height);

private:
	sg_size_t m_height;
	sg_size_t m_width;
};

}

#endif /* SGFX_FONTSVG_HPP_ */
