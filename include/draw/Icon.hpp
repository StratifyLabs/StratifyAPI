/*! \file */ //Copyright 2011-2016 Tyler Gilbert; All Rights Reserved

#ifndef DRAW_ICON_HPP_
#define DRAW_ICON_HPP_

#include "Drawing.hpp"

namespace draw {

/*! \brief Icon Attributes Class
 * \ingroup element
 * \details This class defines the attributes of a Gfx object.
 */

class IconAttr {
public:

	enum sys_gfx {
		OK,
		CHEVRON,
		TOTAL
	};

	/*! \brief Construct with no graphic */
	IconAttr(){ set(TOTAL); }

	/*! \brief Construct by setting the graphic, thickness, and rotation */
	IconAttr(enum sys_gfx gfx, u8 thickness = 3, i16 rotation = 0){
		set(gfx, thickness, rotation);
	}


	/*! \brief Gfx rotation orienations */
	enum {
		RIGHT /*! \brief Point to the right */ = 0,
		DOWN /*! \brief Point down */ = SG_TRIG_POINTS/4,
		LEFT /*! \brief Point to the left */ = SG_TRIG_POINTS/2,
		UP /*! \brief Point up */ = SG_TRIG_POINTS*3/4,
		QUARTER_CLOCKWISE /*! \brief Add/subtract to/from RIGHT, DOWN, etc */ = SG_TRIG_POINTS/4,
		QUARTER_COUNTER_CLOCKWISE /*! \brief Add/subtract to/from RIGHT, DOWN, etc */ = -SG_TRIG_POINTS/4,
		EIGHTH_CLOCKWISE /*! \brief Add/subtract to/from RIGHT, DOWN, etc */  = SG_TRIG_POINTS/8,
		EIGHTH_COUNTER_CLOCKWISE /*! \brief Add/subtract to/from RIGHT, DOWN, etc */  = -SG_TRIG_POINTS/8,
	};

	/*! \brief Set the graphic, thickness, and rotation */
	inline void set(enum sys_gfx gfx, u8 thickness = 3, i16 rotation = 0){
		m_gfx = gfx; m_thickness = thickness; m_rotation = rotation;
	}

	/*! \brief Set the thickness */
	inline void set_thickness(u8 thickness){ m_thickness = thickness; }
	/*! \brief Set the graphic */
	inline void set_gfx(enum sys_gfx gfx){ m_gfx = gfx; }
	/*! \brief Set the rotation */
	inline void set_rotation(i16 rotation){ m_rotation = rotation; }

	/*! \brief Returns the graphic */
	inline enum sys_gfx gfx() const { return m_gfx; }
	/*! \brief Returns the thickness */
	inline u8 thickness() const { return m_thickness; }
	/*! \brief Returns the rotation */
	inline i16 rotation() const { return m_rotation; }

private:
	enum sys_gfx m_gfx;
	u8 m_thickness;
	i16 m_rotation;
};

/*! \brief Gfx Class
 * \ingroup element
 * \details This class draws scalable graphics that can be rotated on the screen.
 */

class Icon : public Drawing {
public:
	/*! \brief Construct an empty graphic */
	Icon();
	/*! \brief Construct a graphic using \a gfx */
	Icon(enum IconAttr::sys_gfx gfx);

	Icon(const IconAttr & gfx);

	IconAttr & icon(){ return m_attr; }
	/*! \brief Access a reference to the attributes */
	IconAttr & attr(){ return m_attr; }

	static const sg_icon_t * load_icon(enum IconAttr::sys_gfx gfx);

	/*! \brief Draws the graphic to scale on the specified bitmap */
	virtual void draw_to_scale(const DrawingScaledAttr & attr);

	sg_bounds_t & bounds(){ return m_bounds; }

private:
	IconAttr m_attr;
	sg_bounds_t m_bounds;

};

};


#endif /* DRAW_ICON_HPP_ */
