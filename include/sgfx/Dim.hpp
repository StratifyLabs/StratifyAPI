/*! \file */ //Copyright 2011-2018 Tyler Gilbert; All Rights Reserved

#ifndef SGFX_DIM_HPP_
#define SGFX_DIM_HPP_

#include <sapi/sg_types.h>
#include "../api/SgfxObject.hpp"

namespace sgfx {

/*! \brief Dimension Class
 * \details This class holds a dimension for bitmaps.
 * It is based on the sgfx library.
 */
class Dim : public api::SgfxInfoObject {
public:
	Dim(){ m_value.height = 0; m_value.width = 0; }
	Dim(sg_size_t w, sg_size_t h){ m_value.width = w; m_value.height = h; }
	Dim(const sg_dim_t & d){ m_value = d; }

	bool is_valid() const {
		return (m_value.width && m_value.height);
	}

	void set_width(sg_size_t w){ m_value.width = w; }
	void set_height(sg_size_t h){ m_value.height = h; }

	Dim operator * (float a) const {
		Dim d;
		d.m_value.width = m_value.width * a;
		d.m_value.height = m_value.height * a;
		return d;
	}

	Dim & operator *= (float a){
		m_value.width *= a;
		m_value.height *= a;
		return *this;
	}

	u32 area() const { return m_value.width * m_value.height; }

	sg_size_t width() const { return m_value.width; }
	sg_size_t height() const { return m_value.height; }

	const sg_dim_t & dim() const { return m_value; }
	sg_dim_t & dim(){ return m_value; }

	operator const sg_dim_t & () const { return m_value; }

private:
	sg_dim_t m_value;
};

}

#endif /* SGFX_DIM_HPP_ */
