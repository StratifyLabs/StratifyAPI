/*
 * Pen.hpp
 *
 *  Created on: Jun 14, 2016
 *      Author: tgil
 */

#ifndef SGFX_PEN_HPP_
#define SGFX_PEN_HPP_


#include <sapi/sg_types.h>
#include "../var/Item.hpp"

namespace sgfx {

/*! \brief Pen Class
 * \details The pen defines the color, thickness
 * and mode when drawing on bitmaps.
 */
class Pen : public var::Item<sg_pen_t> {
public:

	Pen();
	Pen(sg_color_t color, u8 thickness = 1, bool fill = false);


	/*! \details Accesses the pen thickness. */
	u8 thickness() const { return item().thickness; }

	bool is_invert() const { return (item().o_flags & SG_PEN_FLAG_INVERT) != 0; }
	bool is_blend() const { return (item().o_flags & SG_PEN_FLAG_BLEND) != 0; }
	bool is_fill() const { return (item().o_flags & SG_PEN_FLAG_FILL) != 0; }

	/*! \details Access the pen color. */
	sg_color_t color() const { return item().color; }



	/*! \details Sets the pen color.
	 *
	 * @param color The color
	 */
	void set_color(sg_color_t color){ data()->color = color; }

	/*! \details Sets the pen thickness. */
	void set_thickness(u8 v){ data()->thickness = v; }


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
