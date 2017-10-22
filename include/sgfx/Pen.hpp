//Copyright 2011-2016 Tyler Gilbert; All Rights Reserved

#ifndef SGFX_PEN_HPP_
#define SGFX_PEN_HPP_

#include <sapi/sg_types.h>
#include "../var/Item.hpp"

namespace sgfx {

/*! \brief Pen Class
 * \details The pen defines the color, thickness
 * and mode when drawing on bitmaps.
 *
 * Whenever drawing on bitmaps, a pen is associated
 * with the bitmap. The current state of the pen determines
 * how primitives are drawn.
 *
 * First, pens have a color value that is represented
 * by the number of bits linked to the graphics library.
 * For example, when using 1bpp, valid colors values are 0 and 1.
 *
 * Pens can operate in four modes:
 * - Solid: pen color is assigned to bitmap
 * - Blend: pen color is OR'd with bitmap color
 * - Invert: pen color is XOR'd with bitmap color
 * - Erase: pen color is inverted when AND'd with the bitmap color
 *
 * When drawing a source bitmap on a destination bitmap,
 * the pen mode is determined by the destination bitmap's pen.
 * The pen color is ignored and the source bitmap pixel color is used.
 * The source bitmap's pen is not used in bitmap to bitmap drawing operations.
 *
 * Icon vector graphics sometimes include fill points. The destination
 * bitmap's pen determines whether or not the fill operations are performed
 * based on the pen's fill flag (see is_fill() and set_fill()).
 *
 * The pen can also specify a thickness which applies to drawing
 * lines and arcs on bitmaps.
 *
 *
 */
class Pen : public var::Item<sg_pen_t> {
public:

	Pen();
	Pen(sg_color_t color, u8 thickness = 1, bool fill = false);
	Pen(const sg_pen_t & pen){ set(pen); }

	enum {
		FLAG_IS_SOLID /*! Draw solid */ = SG_PEN_FLAG_IS_SOLID,
		FLAG_IS_INVERT /*! Draw Invert (XOR) */ = SG_PEN_FLAG_IS_INVERT,
		FLAG_IS_BLEND /*! Draw Blend (OR) */ = SG_PEN_FLAG_IS_BLEND,
		FLAG_IS_ERASE /*! Draw Erase (NOT AND) */ = SG_PEN_FLAG_IS_ERASE,
	};


	/*! \details Accesses the pen thickness. */
	u8 thickness() const { return item().thickness; }

	bool is_solid() const { return (item().o_flags & SG_PEN_FLAG_NOT_SOLID_MASK) == 0; }
	/*! \details Returns true if pen is in invert mode. */
	bool is_invert() const { return (item().o_flags & SG_PEN_FLAG_IS_INVERT) != 0; }
	/*! \details Returns true if pen is in blend mode. */
	bool is_blend() const { return (item().o_flags & SG_PEN_FLAG_IS_BLEND) != 0; }
	/*! \details Returns true if pen is in erase mode. */
	bool is_erase() const { return (item().o_flags & SG_PEN_FLAG_IS_ERASE) != 0; }

	/*! \details Returns true if fill is enabled for pen (used with icons). */
	bool is_fill() const { return (item().o_flags & SG_PEN_FLAG_IS_FILL) != 0; }

	/*! \details Access the pen color. */
	sg_color_t color() const { return item().color; }

	/*! \details Sets the pen to a solid color mode (assign value). */
	void set_solid(){ data()->o_flags &= ~SG_PEN_FLAG_IS_INVERT; }

	/*! \details Sets the pen to inverting color mode (XOR). */
	void set_invert(){ set_solid(); data()->o_flags |= SG_PEN_FLAG_IS_INVERT; }

	/*! \details Sets the pen to a clearing mode (AND). */
	void set_erase(){ set_solid(); data()->o_flags |= SG_PEN_FLAG_IS_ERASE; }

	/*! \details Sets the pen to a blending mode (OR). */
	void set_blend(){ set_solid(); data()->o_flags |= SG_PEN_FLAG_IS_INVERT; }


	/*! \details Sets the pen color.
	 *
	 * @param color The color
	 */
	void set_color(sg_color_t color){ data()->color = color; }

	/*! \details Sets the pen thickness. */
	void set_thickness(u8 v){ data()->thickness = v; }

	/*! \details Sets the pen to fill when drawing vector icons. */
	void set_fill(bool v = true){
		if( v ){
			data()->o_flags |= SG_PEN_FLAG_IS_FILL;
		} else {
			data()->o_flags &= ~SG_PEN_FLAG_IS_FILL;
		}
	}

};

} /* namespace sgfx */

#endif /* SGFX_PEN_HPP_ */
