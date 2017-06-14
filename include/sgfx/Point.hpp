/*! \file */ //Copyright 2011-2016 Tyler Gilbert; All Rights Reserved

#ifndef SGFX_POINT_HPP_
#define SGFX_POINT_HPP_

#include <sgfx/sg.h>

namespace sgfx {

/*! \brief Point Class
 * \details The Point class holds a point on a Bitmap.  This
 * class is based on teh sgfx library.
 *
 */
class Point {
public:
	Point(){ d.x = 0; d.y = 0; }
	Point(sg_point_t p){ d = p; }
	Point(const sg_vector_primitive_t & a);
	Point(sg_int_t x, sg_int_t y){ d.x = x; d.y = y; }

	sg_point_t point() const { return d; }

	operator sg_point_t(){ return d; }


	inline void set(sg_int_t x, sg_int_t y){ d.x = x; d.y = y; }

	inline sg_int_t x() const { return d.x; }
	inline sg_int_t y() const { return d.y; }


	void map(const sg_vector_map_t & m){ sg_point_map(&d, &m); }

	static sg_size_t map_pixel_size(const sg_vector_map_t & m){ return sg_point_map_pixel_size(&m); }

	Point & operator=(const sg_point_t & a){ d = a; return *this; }
	Point & operator+(const sg_point_t & a){ sg_api()->point_shift(&d, a); return *this; }
	Point & operator-(const sg_point_t & a){ sg_api()->point_subtract(&d, &a); return *this; }

	void rotate(i16 angle){ sg_api()->point_rotate(&d, angle); }
	void scale(u16 a){ sg_api()->point_scale(&d, a); }
	void shift(i16 x, i16 y){ sg_api()->point_shift(&d, sg_point(x,y)); }
	void shift(sg_point_t p){ sg_api()->point_shift(&d, p); }

private:
	sg_point_t d;
};

};




#endif /* SGFX_POINT_HPP_ */
