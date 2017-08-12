/*! \file */ //Copyright 2011-2017 Tyler Gilbert; All Rights Reserved


#ifndef DRAW_IMAGE_HPP_
#define DRAW_IMAGE_HPP_

#include "Drawing.hpp"


namespace draw {

class Image : public Drawing {
public:
	Image();
	Image(sgfx::Bitmap & bitmap);
	void draw_to_scale(const DrawingScaledAttr & attr);

	drawing_dim_t calc_dim();

	void set_bitmap(const sgfx::Bitmap * bitmap){
		m_bitmap = bitmap;
	}

private:
	const sgfx::Bitmap * m_bitmap;
};

} /* namespace draw */

#endif /* DRAW_IMAGE_HPP_ */
