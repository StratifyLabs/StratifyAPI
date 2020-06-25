/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see LICENSE.md for rights.

#ifndef SAPI_SGFX_POINT_HPP_
#define SAPI_SGFX_POINT_HPP_

#include <sapi/sg.h>
#include "../api/SgfxObject.hpp"
#include "Area.hpp"

namespace sgfx {

/*! \brief Point Class
 * \details The Point class holds a point on a Bitmap.  This
 * class is based on teh sgfx library.
 *
 */
class Point : public api::SgfxInfoObject {
public:

	using X = arg::Argument<sg_int_t, struct PointXTag>;
	using Y = arg::Argument<sg_int_t, struct PointYTag>;
	using ImplicitX = arg::ImplicitArgument<sg_int_t, struct PointImplicitXTag, X>;
	using ImplicitY = arg::ImplicitArgument<sg_int_t, struct PointImplicitYTag, Y>;


	Point(){ m_value.point = 0; }
	Point(const sg_point_t & p) : m_value(p) {}
	Point(
			const ImplicitX x,
			const ImplicitY y
			){
		m_value.x = x.argument();
		m_value.y = y.argument();
	}

	const sg_point_t & point() const { return m_value; }
	sg_point_t & point(){ return m_value; }

	operator const sg_point_t & () const { return m_value; }

	Point & set_x(sg_int_t value){
		m_value.x = value;
		return *this;
	}

	Point & set_y(sg_int_t value){
		m_value.y = value;
		return *this;
	}

	Point & set(
			X x,
			Y y
			){
		m_value.x = x.argument();
		m_value.y = y.argument();
		return *this;
	}

	sg_int_t x() const { return m_value.x; }
	sg_int_t y() const { return m_value.y; }


	void map(const sg_vector_map_t & m){ api()->point_map(&m_value, &m); }
	void unmap(const sg_vector_map_t & m){ api()->point_unmap(&m_value, &m); }

	static sg_size_t map_pixel_size(const sg_vector_map_t & m){ return sg_point_map_pixel_size(&m); }

	Point & operator=(const Point & a){ m_value = a; return *this; }
	bool operator==(const Point & a) const { return m_value.point == a.m_value.point; }
	bool operator!=(const Point & a) const { return m_value.point != a.m_value.point; }
	bool operator>(const Point & a) const { return m_value.point > a.m_value.point; }
	bool operator>=(const Point & a)const { return m_value.point >= a.m_value.point; }
	bool operator<=(const Point & a) const { return m_value.point <= a.m_value.point; }
	bool operator<(const Point & a) const { return m_value.point < a.m_value.point; }

	Point & operator+=(const Point & a){ api()->point_shift(&m_value, a); return *this; }
	Point & operator+=(const X x);
	Point & operator+=(const Y y);
	Point & operator-=(const Point & a){ api()->point_subtract(&m_value, &a.point()); return *this; }
	Point & operator-=(const X x);
	Point & operator-=(const Y y);
	Point operator*(float f) const;
	Point operator+(const Point & a) const;
	Point operator+(const X x) const;
	Point operator+(const Y y) const;
	Point operator-(const Point & a) const;
	Point operator-(const X x) const;
	Point operator-(const Y y) const;

	void rotate(s16 angle){ api()->point_rotate(&m_value, angle); }
	void scale(u16 a){ api()->point_scale(&m_value, a); }
	void shift(s16 x, s16 y){ api()->point_shift(&m_value, sg_point(x,y)); }
	void shift(sg_point_t p){ api()->point_shift(&m_value, p); }



private:
	sg_point_t m_value;
};

}

#endif /* SAPI_SGFX_POINT_HPP_ */
