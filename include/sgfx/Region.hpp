/*! \file */ //Copyright 2011-2016 Tyler Gilbert; All Rights Reserved

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
	Region(){}
	Region(sg_int_t x, sg_int_t y, sg_size_t w, sg_size_t h){
		m_region.point.x = x;
		m_region.point.y = y;
		m_region.dim.width = w;
		m_region.dim.height = h;
	}


	Region(const sg_region_t & region){
		m_region = region;
	}

	Region(const Point & point, const Dim & dim){
		m_region.point = point;
		m_region.dim = dim;
	}

	void set_point(const Point & value){ m_region.point = value; }
	void set_dim(const Dim & value){ m_region.dim = value; }

	Point point() const { return m_region.point; }
	Dim dim() const { return m_region.dim; }


	sg_region_t region() const { return m_region; }
	operator sg_region_t() const { return m_region; }

	sg_int_t x() const { return m_region.point.x; }
	sg_int_t y() const { return m_region.point.y; }
	sg_int_t width() const { return m_region.dim.width; }
	sg_int_t height() const { return m_region.dim.height; }


private:
	sg_region_t m_region;
};

}

#endif /* SGFX_REGION_HPP_ */
