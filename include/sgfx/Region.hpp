/*! \file */ //Copyright 2011-2018 Tyler Gilbert; All Rights Reserved

#ifndef SGFX_REGION_HPP_
#define SGFX_REGION_HPP_

#include <sapi/sg_types.h>

#include "Point.hpp"
#include "Dim.hpp"
#include "../api/SgfxObject.hpp"

namespace sgfx {

/*! \brief Region Class
 * \details The Region class creates a region
 * in the graphics space defined by a point (top-left corner)
 * and a dimension.
 */
class Region : public api::SgfxInfoObject {
public:
	Region(){
		memset(&m_region, 0, sizeof(m_region));
	}

	bool is_valid() const {
		return (m_region.dim.width != 0) && (m_region.dim.height != 0);
	}


	Region(const sg_region_t & region){
		m_region = region;
	}

	Region(const Point & point, const Dim & dim){
		m_region.point = point;
		m_region.dim = dim;
	}

	Point center() const {
		return Point(m_region.point.x + m_region.dim.width/2,
						 m_region.point.y + m_region.dim.height/2);
	}

	void set_region(const Point & top_left, const Point & bottom_right){
		m_region.point = top_left;
		m_region.dim.width = bottom_right.x() - top_left.x() + 1;
		m_region.dim.height = bottom_right.y() - top_left.y() + 1;
	}

	void set_point(const Point & value){ m_region.point = value; }
	void set_dim(const Dim & value){ m_region.dim = value; }

	Point point() const { return m_region.point; }
	Dim dim() const { return m_region.dim; }

	sg_int_t x() const { return m_region.point.x; }
	sg_int_t y() const { return m_region.point.y; }
	sg_size_t width() const { return m_region.dim.width; }
	sg_size_t height() const { return m_region.dim.height; }

	Region & operator << (const Point & point){
		set_point(point);
		return *this;
	}

	Region & operator << (const Dim & dimension){
		set_dim(dimension);
		return *this;
	}

	const sg_region_t & region() const { return m_region; }
	sg_region_t region(){ return m_region; }

	operator	const sg_region_t & () const { return m_region; }

private:
	sg_region_t m_region;
};

}

#endif /* SGFX_REGION_HPP_ */
