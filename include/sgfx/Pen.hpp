/*
 * Pen.hpp
 *
 *  Created on: Jun 14, 2016
 *      Author: tgil
 */

#ifndef SGFX_PEN_HPP_
#define SGFX_PEN_HPP_


#include <sgfx/sg_types.h>
#include "../var/Item.hpp"

namespace sgfx {

class Pen : public var::Item<sg_pen_t> {
public:

	enum mode {
		NONE = 0,
		INVERT /*! Invert pixels */ = SG_PEN_FLAG_INVERT,
		BLEND /*! Blend pixels */ = SG_PEN_FLAG_BLEND,
	};

	Pen();
	Pen(sg_color_t color, u8 thickness = 1, bool fill = false);


	u8 thickness() const { return item().thickness; }
	bool is_invert() const { return (item().o_flags & SG_PEN_FLAG_INVERT) != 0; }
	bool is_blend() const { return (item().o_flags & SG_PEN_FLAG_BLEND) != 0; }
	bool is_fill() const { return (item().o_flags & SG_PEN_FLAG_FILL) != 0; }
	sg_color_t color() const { return item().color; }




	void set_color(u32 v){ data()->color = v; }

	void set_thickness(u8 v){ data()->thickness = v; }
	void set_mode(enum mode v){
		u16 flags = data()->o_flags;
		data()->o_flags = (flags & SG_PEN_FLAG_FILL) | v;
	}
	void set_fill(bool v = true){
		if( v ){
			data()->o_flags |= SG_PEN_FLAG_FILL;
		} else {
			data()->o_flags &= ~SG_PEN_FLAG_FILL;
		}
	}

};

} /* namespace sgfx */

#endif /* SGFX_PEN_HPP_ */
