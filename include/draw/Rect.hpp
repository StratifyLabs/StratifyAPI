/*! \file */ //Copyright 2011-2016 Tyler Gilbert; All Rights Reserved

#ifndef DRAW_RECT_HPP_
#define DRAW_RECT_HPP_

#include "Drawing.hpp"

namespace draw {

/*! \brief Rect Class
 * \ingroup element
 * \details The Rect class draws a rectangle on the display.  It can be used for
 * both horizontal and vertical separators as well.
 */
class Rect : public Drawing {
public:
	Rect();
	virtual void draw_to_scale(const DrawingScaledAttr & attr);

};

}

#endif /* DRAW_RECT_HPP_ */
