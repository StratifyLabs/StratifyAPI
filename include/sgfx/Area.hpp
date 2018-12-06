/*! \file */ //Copyright 2011-2018 Tyler Gilbert; All Rights Reserved

#ifndef SAPI_SGFX_AREA_HPP_
#define SAPI_SGFX_AREA_HPP_

#include <sapi/sg_types.h>
#include "../api/SgfxObject.hpp"

namespace sgfx {

/*! \brief Areaension Class
 * \details This class holds a dimension for bitmaps.
 * It is based on the sgfx library.
 */
class Area : public api::SgfxInfoObject {
public:
	Area();
	Area(sg_size_t w, sg_size_t h){ m_value.width = w; m_value.height = h; }
	Area(const sg_area_t & d){ m_value = d; }

	bool is_valid() const {
		return (m_value.width && m_value.height);
	}

	void set_width(sg_size_t w){ m_value.width = w; }
	void set_height(sg_size_t h){ m_value.height = h; }

	Area operator * (float a) const {
		Area d;
		d.m_value.width = m_value.width * a;
		d.m_value.height = m_value.height * a;
		return d;
	}

	Area & operator *= (float a){
		m_value.width *= a;
		m_value.height *= a;
		return *this;
	}

	u32 calculate_area() const { return m_value.width * m_value.height; }

	sg_size_t width() const { return m_value.width; }
	sg_size_t height() const { return m_value.height; }

	sg_size_t maximum_dimension() const {
		return m_value.width > m_value.height ? m_value.width : m_value.height;
	}

	sg_size_t minimum_dimension() const {
		return m_value.width < m_value.height ? m_value.width : m_value.height;
	}

	const sg_area_t & area() const { return m_value; }
	sg_area_t & area(){ return m_value; }

	operator const sg_area_t & () const { return m_value; }

private:
	sg_area_t m_value;
};

}

#endif /* SAPI_SGFX_AREA_HPP_ */
