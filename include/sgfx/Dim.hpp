/*! \file */ //Copyright 2011-2016 Tyler Gilbert; All Rights Reserved

#ifndef SGFX_DIM_HPP_
#define SGFX_DIM_HPP_

#include <sapi/sg_types.h>

namespace sgfx {

/*! \brief Dimension Class
 * \details This class holds a dimension for bitmaps.
 * It is based on the sgfx library.
 */
class Dim {
public:
	Dim(){ m_value.height = 0; m_value.width = 0; }
	Dim(sg_size_t w, sg_size_t h){ m_value.width = w; m_value.height = h; }
	Dim(sg_dim_t d){ m_value = d; }

	operator sg_dim_t() const { return m_value; }
	sg_dim_t dim() const { return m_value; }
	sg_dim_t value() const { return m_value; }

	void set_value(sg_dim_t v){ m_value = v; }
	void set_value(sg_size_t w, sg_size_t h){ m_value.width = w; m_value.height = h; }
	void set_width(sg_size_t w){ m_value.width = w; }
	void set_height(sg_size_t h){ m_value.height = h; }

	u32 size() const { return m_value.width * m_value.height; }

	sg_size_t width() const { return m_value.width; }
	sg_size_t height() const { return m_value.height; }

private:
	sg_dim_t m_value;
};

};




#endif /* SGFX_DIM_HPP_ */
