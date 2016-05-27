/*! \file */ //Copyright 2011-2016 Tyler Gilbert; All Rights Reserved

#ifndef SGFX_DIM_HPP_
#define SGFX_DIM_HPP_

#include <sgfx/sg_types.h>

namespace sgfx {

/*! \brief Dimension Class
 * \details This class holds a dimension for bitmaps.
 * It is based on the sgfx library.
 */
class Dim {
public:
	Dim(){ m_value.h = 0; m_value.w = 0; }
	Dim(sg_size_t w, sg_size_t h){ m_value.w = w; m_value.h = h; }
	Dim(sg_dim_t d){ m_value = d; }

	operator sg_dim_t() const { return m_value; }
	sg_dim_t dim() const { return m_value; }
	sg_dim_t value() const { return m_value; }

	inline void set_value(sg_dim_t v){ m_value = v; }
	inline void set_value(sg_size_t w, sg_size_t h){ m_value.w = w; m_value.h = h; }
	inline void set_w(sg_size_t w){ m_value.w = w; }
	inline void set_h(sg_size_t h){ m_value.h = h; }

	inline u32 size() const { return m_value.w * m_value.h; }

	inline sg_size_t w() const { return m_value.w; }
	inline sg_size_t h() const { return m_value.h; }

private:
	sg_dim_t m_value;
};

};




#endif /* SGFX_DIM_HPP_ */
