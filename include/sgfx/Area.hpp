/*! \file */ //Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc

#ifndef SAPI_SGFX_AREA_HPP_
#define SAPI_SGFX_AREA_HPP_

#include <sapi/sg_types.h>
#include "../api/SgfxObject.hpp"
#include "../arg/Argument.hpp"

namespace sgfx {

/*! \brief Areaension Class
 * \details This class holds a dimension for bitmaps.
 * It is based on the sgfx library.
 */
class Area : public api::SgfxInfoObject {
public:


	using Width = arg::Argument<sg_size_t, struct AreaWidthTag>;
	using Height = arg::Argument<sg_size_t, struct AreaHeightTag>;
	using ImplicitWidth = arg::ImplicitArgument<sg_size_t, struct ImplicitWidthTag, Width>;
	using ImplicitHeight = arg::ImplicitArgument<sg_size_t, struct ImplicitHeightTag, Height>;

	Area();
	Area(
			ImplicitWidth width,
			ImplicitHeight height
			){
		m_value.width = width.argument();
		m_value.height = height.argument();
	}

	Area(const sg_area_t & d){ m_value = d; }

	bool is_valid() const {
		return (m_value.width && m_value.height);
	}

	Area & set_width(sg_size_t w){ m_value.width = w; return *this; }
	Area & set_height(sg_size_t h){ m_value.height = h; return *this; }


	bool operator == (const Area & a) const {
		return m_value.area == a.m_value.area;
	}

	bool operator != (const Area & a) const {
		return m_value.area != a.m_value.area;
	}

	bool operator > (const Area & a) const {
		return m_value.width*m_value.height >
				a.m_value.width*a.m_value.height;
	}

	bool operator < (const Area & a) const {
		return m_value.width*m_value.height <
				a.m_value.width*a.m_value.height;
	}


	bool operator >= (const Area & a) const {
		return m_value.width*m_value.height >=
				a.m_value.width*a.m_value.height;
	}

	bool operator <= (const Area & a) const {
		return m_value.width*m_value.height <=
				a.m_value.width*a.m_value.height;
	}


	Area operator * (float a) const {
		Area d;
		d.m_value.width = m_value.width * a;
		d.m_value.height = m_value.height * a;
		return d;
	}

	Area operator * (sg_size_t a) const {
		Area d;
		d.m_value.width = m_value.width * a;
		d.m_value.height = m_value.height * a;
		return d;
	}

	Area operator / (sg_size_t a) const {
		Area d;
		d.m_value.width = m_value.width / a;
		d.m_value.height = m_value.height / a;
		return d;
	}

	Area & operator *= (float a){
		m_value.width *= a;
		m_value.height *= a;
		return *this;
	}

	Area operator + (const Area & a){
		return Area(
					width() + a.width(),
					height() + a.height()
					);
	}

	Area operator - (const Area & a){
		return Area(
					width() - a.width(),
					height() - a.height()
					);
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
