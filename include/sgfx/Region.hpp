/*! \file */ //Copyright 2011-2018 Tyler Gilbert; All Rights Reserved

#ifndef SAPI_SGFX_REGION_HPP_
#define SAPI_SGFX_REGION_HPP_

#include <sapi/sg_types.h>

#include "Point.hpp"
#include "Area.hpp"
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
		return (m_region.area.width != 0) && (m_region.area.height != 0);
	}


	Region(const sg_region_t & region){
		m_region = region;
	}

	Region(const Point & point, const Area & area){
		m_region.point = point;
		m_region.area = area;
	}

	Point center() const {
		return Point(
					Point::X(m_region.point.x + m_region.area.width/2),
					Point::Y(m_region.point.y + m_region.area.height/2)
					);
	}

	Point center_point() const {
		return Point(
					Point::X(m_region.point.x + m_region.area.width/2),
					Point::Y(m_region.point.y + m_region.area.height/2)
					);
	}

	Point end_point() const {
		return Point(
					Point::X(m_region.point.x + m_region.area.width),
					Point::Y(m_region.point.y + m_region.area.height)
					);
	}

	void set_region(const Point & top_left, const Point & bottom_right){
		m_region.point = top_left;
		m_region.area.width = bottom_right.x() - top_left.x() + 1;
		m_region.area.height = bottom_right.y() - top_left.y() + 1;
	}

	Region & set_point(const Point & value){ m_region.point = value; return *this; }
	Region & set_area(const Area & value){ m_region.area = value; return *this; }

	Point point() const { return m_region.point; }
	Area area() const { return m_region.area; }

	sg_int_t x() const { return m_region.point.x; }
	sg_int_t y() const { return m_region.point.y; }
	sg_size_t width() const { return m_region.area.width; }
	sg_size_t height() const { return m_region.area.height; }

	Region & operator << (const Point & point){
		return set_point(point);
	}

	Region & operator << (const Area & dimension){
		return set_area(dimension);
	}

	const sg_region_t & region() const { return m_region; }
	sg_region_t region(){ return m_region; }

	operator	const sg_region_t & () const { return m_region; }

	bool contains(const Point & point) const {
		if( point.x() < x() ){ return false; }
		if( point.y() < y() ){ return false; }
		if( point.x() > x() + width()-1){ return false; }
		if( point.y() > y() + height()-1){ return false; }
		return true;
	}

	Region overlap(const Region & region){
		Region result;
		{
			sg_int_t w;
			if( region.x() < x() ){
				result.m_region.point.x = x() - region.x();
				w = region.x() + region.width() - x();
				if( w < 0 ){
					w = 0;
				}
			} else if( region.x() > x() + width() ){
				result.m_region.point.x = 0;
				w = 0;
			} else {
				result.m_region.point.x = 0;
				w = x() + width() - region.x();
				if( w > region.width() ){
					w = region.width();
				}
			}
			result.m_region.area.width = w;
		}

		{
			sg_int_t h;
			if( region.y() < y() ){
				result.m_region.point.y = y() - region.y();
				h = region.y() + region.height() - y();
				if( h < 0 ){
					h = 0;
				}
			} else if( region.y() > y() + height() ){
				result.m_region.point.y = 0;
				h = 0;
			} else {
				result.m_region.point.y = 0;
				h = y() + height() - region.y();
				if( h > region.height() ){
					h = region.height();
				}
			}
			result.m_region.area.height = h;
		}


		return result;
	}

private:
	sg_region_t m_region;
};

}

#endif /* SAPI_SGFX_REGION_HPP_ */
