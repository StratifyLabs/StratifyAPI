/*! \file */ //Copyright 2011-2016 Tyler Gilbert; All Rights Reserved

#ifndef SGFX_POINT_HPP_
#define SGFX_POINT_HPP_

#include <sapi/sg.h>

namespace sgfx {

/*! \brief Point Class
 * \details The Point class holds a point on a Bitmap.  This
 * class is based on teh sgfx library.
 *
 */
class Point {
public:
	Point(){ m_value.x = 0; m_value.y = 0; }
	Point(sg_point_t p){ m_value = p; }
	Point(sg_int_t x, sg_int_t y){ m_value.x = x; m_value.y = y; }

	sg_point_t point() const { return m_value; }
	operator sg_point_t() const { return m_value; }

	inline void set(sg_int_t x, sg_int_t y){ m_value.x = x; m_value.y = y; }

	inline sg_int_t x() const { return m_value.x; }
	inline sg_int_t y() const { return m_value.y; }

	void set_center(const sg_bmap_t * bmap){
		m_value.x = bmap->dim.width/2;
		m_value.y = bmap->dim.height/2;
	}

	void set_center(const sg_bmap_t & bmap){
		m_value.x = bmap.dim.width/2;
		m_value.y = bmap.dim.height/2;
	}


	void map(const sg_vector_map_t & m){ sg_api()->point_map(&m_value, &m); }

	static sg_size_t map_pixel_size(const sg_vector_map_t & m){ return sg_point_map_pixel_size(&m); }

	Point & operator=(const sg_point_t & a){ m_value = a; return *this; }
	Point & operator+=(const sg_point_t & a){ sg_api()->point_shift(&m_value, a); return *this; }
	Point operator*(float f) const;
	Point operator+(const sg_point_t & a) const;
	Point operator-(const sg_point_t & a) const;

	void rotate(s16 angle){ sg_api()->point_rotate(&m_value, angle); }
	void scale(u16 a){ sg_api()->point_scale(&m_value, a); }
	void shift(s16 x, s16 y){ sg_api()->point_shift(&m_value, sg_point(x,y)); }
	void shift(sg_point_t p){ sg_api()->point_shift(&m_value, p); }

private:
	sg_point_t m_value;
};

};




#endif /* SGFX_POINT_HPP_ */
