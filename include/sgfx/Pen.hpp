//Copyright 2011-2018 Tyler Gilbert; All Rights Reserved

#ifndef SAPI_SGFX_PEN_HPP_
#define SAPI_SGFX_PEN_HPP_

#include <sapi/sg_types.h>
#include "../api/SgfxObject.hpp"
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
class Pen : public api::SgfxInfoObject {
public:

	Pen();
	Pen(sg_color_t color);
	Pen(sg_color_t color, u8 thickness, bool fill);
	Pen(sg_color_t color, u16 o_flags);
	Pen(const sg_pen_t & pen){ m_pen = pen; }

	enum {
		IS_SOLID /*! Draw solid */ = SG_PEN_FLAG_IS_SOLID,
		IS_INVERT /*! Draw Invert (XOR) */ = SG_PEN_FLAG_IS_INVERT,
		IS_BLEND /*! Draw Blend (OR) */ = SG_PEN_FLAG_IS_BLEND,
		IS_ERASE /*! Draw Erase (NOT AND) */ = SG_PEN_FLAG_IS_ERASE,
		IS_FILL /*! Draw Icon with fill points */ = SG_PEN_FLAG_IS_FILL
	};


	/*! \details Accesses the pen thickness. */
	u8 thickness() const { return m_pen.thickness; }

	bool is_solid() const { return (m_pen.o_flags & SG_PEN_FLAG_NOT_SOLID_MASK) == 0; }
	/*! \details Returns true if pen is in invert mode. */
	bool is_invert() const { return (m_pen.o_flags & SG_PEN_FLAG_IS_INVERT) != 0; }
	/*! \details Returns true if pen is in blend mode. */
	bool is_blend() const { return (m_pen.o_flags & SG_PEN_FLAG_IS_BLEND) != 0; }
	/*! \details Returns true if pen is in erase mode. */
	bool is_erase() const { return (m_pen.o_flags & SG_PEN_FLAG_IS_ERASE) != 0; }

	/*! \details Returns true if fill is enabled for pen (used with icons). */
	bool is_fill() const { return (m_pen.o_flags & SG_PEN_FLAG_IS_FILL) != 0; }

	/*! \details Access the pen color. */
	sg_color_t color() const { return m_pen.color; }

	/*! \details Sets the pen to a solid color mode (assign value). */
	Pen & set_solid(){ m_pen.o_flags &= ~SG_PEN_FLAG_NOT_SOLID_MASK; return *this; }

	/*! \details Sets the pen to inverting color mode (XOR). */
	Pen & set_invert(){ set_solid(); m_pen.o_flags |= SG_PEN_FLAG_IS_INVERT; return *this; }

	/*! \details Sets the pen to a clearing mode (AND). */
	Pen & set_erase(){ set_solid(); m_pen.o_flags |= SG_PEN_FLAG_IS_ERASE; return *this; }

	/*! \details Sets the pen to a blending mode (OR). */
	Pen & set_blend(){ set_solid(); m_pen.o_flags |= SG_PEN_FLAG_IS_INVERT; return *this; }

	u16 o_flags() const { return m_pen.o_flags; }
	u16 flags() const { return m_pen.o_flags; }
	Pen & set_flags(u16 flags){	m_pen.o_flags = flags; return *this; }

	/*! \details Sets the pen color.
	 *
	 * @param color The color
	 */
	Pen & set_color(sg_color_t color){ m_pen.color = color; return *this; }

	/*! \details Sets the pen thickness. */
	Pen & set_thickness(u8 v){ m_pen.thickness = v; return *this; }

	/*! \details Sets the pen to fill when drawing vector icons. */
	Pen & set_fill(bool v = true){
		if( v ){
			m_pen.o_flags |= SG_PEN_FLAG_IS_FILL;
		} else {
			m_pen.o_flags &= ~SG_PEN_FLAG_IS_FILL;
		}
		return *this;
	}


	const sg_pen_t & pen() const { return m_pen; }
	sg_pen_t & pen(){ return m_pen; }
	operator const sg_pen_t & () const { return m_pen; }

private:
	sg_pen_t m_pen;

};

} /* namespace sgfx */

#endif /* SAPI_SGFX_PEN_HPP_ */
