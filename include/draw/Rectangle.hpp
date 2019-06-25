/*! \file */ //Copyright 2011-2018 Tyler Gilbert; All Rights Reserved

#ifndef DRAW_RECTANGLE_HPP_
#define DRAW_RECTANGLE_HPP_

#include "Drawing.hpp"

namespace draw {

/*! \brief Rect Class
 * \ingroup element
 * \details The Rect class draws a rectangle on the display.  It can be used for
 * both horizontal and vertical separators as well.
 */
class Rectangle : public Drawing {
public:
	Rectangle();
	virtual void draw_to_scale(const DrawingScaledAttributes & attr);

	Rectangle & set_color(sg_color_t value){
		Drawing::set_color(value);
		return *this;
	}


private:
	sg_color_t m_color;
};

/*! \brief Rect Class
 * \ingroup element
 * \details The Rect class draws a rectangle on the display.  It can be used for
 * both horizontal and vertical separators as well.
 */
class RoundedRectangle : public Drawing {
public:
	RoundedRectangle();
	virtual void draw_to_scale(const DrawingScaledAttributes & attr);

	enum corners {
		TOP_LEFT_ROUNDED = 0,
		TOP_LEFT_SQUARE = (1<<0),
		TOP_RIGHT_ROUNDED = 0,
		TOP_RIGHT_SQUARE = (1<<1),
		BOTTOM_LEFT_ROUNDED = 0,
		BOTTOM_LEFT_SQUARE = (1<<2),
		BOTTOM_RIGHT_ROUNDED = 0,
		BOTTOM_RIGHT_SQUARE = (1<<3),
	};

	RoundedRectangle & set_corners(u8 corners){
		m_corners = corners;
		return *this;
	}

	RoundedRectangle & set_radius(sg_size_t radius){
		m_radius = radius;
		return *this;
	}

	RoundedRectangle & set_color(sg_color_t value){
		Drawing::set_color(value);
		return *this;
	}

private:
	u8 m_corners;
	sg_size_t m_radius;
};


typedef Rectangle Rect;

}

#endif /* DRAW_RECTANGLE_HPP_ */
