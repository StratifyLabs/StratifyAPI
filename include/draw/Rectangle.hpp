/*! \file */ //Copyright 2011-2019 Tyler Gilbert; All Rights Reserved

#ifndef SAPI_DRAW_RECTANGLE_HPP_
#define SAPI_DRAW_RECTANGLE_HPP_

#include "Drawing.hpp"

namespace draw {

/*! \brief Rectangle Class
 * \ingroup element
 * \details The Rect class draws a rectangle on the display.  It can be used for
 * both horizontal and vertical separators as well.
 *
 * The following code snippet will draw a rectangle using color 1 in the
 * middle of the bitmap specified by attributes.
 *
 * ```
 * #include <sapi/draw.hpp>
 * DrawingAttributes attributes;
 *
 * //construct attributes
 *
 * Rectangle().set_color(1).draw(attributes + DrawingPoint(250,250) + DrawingArea(500,500));
 * ```
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
	/*! \cond */
	u8 m_corners;
	sg_size_t m_radius;
	/*! \endcond */

};


typedef Rectangle Rect;

}

#endif /* SAPI_DRAW_RECTANGLE_HPP_ */
