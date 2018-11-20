/*! \file */ //Copyright 2011-2018 Tyler Gilbert; All Rights Reserved

#ifndef SGFX_POINT_HPP_
#define SGFX_POINT_HPP_

#include <sapi/sg.h>
#include "../api/SgfxObject.hpp"

namespace sgfx {

/*! \brief Point Class
 * \details The Point class holds a point on a Bitmap.  This
 * class is based on teh sgfx library.
 *
 */
class Point : public api::SgfxInfoObject {
public:
	Point(){ m_value.x = 0; m_value.y = 0; }
	Point(const sg_point_t & p){ m_value = p; }
	Point(sg_int_t x, sg_int_t y){ m_value.x = x; m_value.y = y; }

	const sg_point_t & point() const { return m_value; }
	sg_point_t & point(){ return m_value; }
	operator const sg_point_t & () const { return m_value; }

	void set(sg_int_t x, sg_int_t y){ m_value.x = x; m_value.y = y; }

	sg_int_t x() const { return m_value.x; }
	sg_int_t y() const { return m_value.y; }


	void map(const sg_vector_map_t & m){ api()->point_map(&m_value, &m); }

	static sg_size_t map_pixel_size(const sg_vector_map_t & m){ return sg_point_map_pixel_size(&m); }

	Point & operator=(const Point & a){ m_value = a; return *this; }
	Point & operator+=(const Point & a){ api()->point_shift(&m_value, a); return *this; }
	Point & operator-=(const Point & a){ api()->point_subtract(&m_value, &a.point()); return *this; }
	Point operator*(float f) const;
	Point operator+(const Point & a) const;
	Point operator-(const Point & a) const;

	void rotate(s16 angle){ api()->point_rotate(&m_value, angle); }
	void scale(u16 a){ api()->point_scale(&m_value, a); }
	void shift(s16 x, s16 y){ api()->point_shift(&m_value, sg_point(x,y)); }
	void shift(sg_point_t p){ api()->point_shift(&m_value, p); }



private:
	sg_point_t m_value;
};

}

#endif /* SGFX_POINT_HPP_ */
