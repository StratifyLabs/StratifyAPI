/*! \file */ //Copyright 2011-2016 Tyler Gilbert; All Rights Reserved

#ifndef DRAW_ICON_HPP_
#define DRAW_ICON_HPP_


#include "Drawing.hpp"
#include "../sgfx/Gfx.hpp"

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

	/*! \details Construct with no graphic */
	IconAttr(){ set_value(0, sgfx::Pen()); }

	/*! \details Construct by setting the graphic, thickness, and rotation */
	IconAttr(const sg_icon_t * icon, u8 thickness = 3, i16 rotation = 0){
		set_value(icon, thickness, rotation);
	}


	/*! \details Icon rotation orientations */
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

	/*! \details Set the graphic, thickness, and rotation */
	void set_value(const sg_icon_t * icon, const sgfx::Pen & pen, i16 rotation = 0){
		m_icon = icon; m_rotation = rotation; m_pen = pen;
	}

	sgfx::Pen & pen(){ return m_pen; }
	const sgfx::Pen & pen_const() const { return m_pen; }

	/*! \details Set the icon */
	void set_icon(const sg_icon_t * icon){ m_icon = icon; }

	/*! \details Set the rotation */
	void set_rotation(i16 rotation){ m_rotation = rotation; }

	/*! \details Returns the graphic */
	const sg_icon_t & icon() const { return *m_icon; }


	/*! \details Returns the rotation */
	s16 rotation() const { return m_rotation; }


private:
	const sg_icon_t * m_icon;
	sgfx::Pen m_pen;
	s16 m_rotation;
};

/*! \brief Gfx Class
 * \ingroup element
 * \details This class draws scalable graphics that can be rotated on the screen.
 */

class Icon : public Drawing {
public:
	/*! \details Construct an empty graphic */
	Icon();

	/*! \details Access a reference to the attributes */
	IconAttr & attr(){ return m_attr; }

	static const sg_icon_t * get_system_icon(int icon);

	/*! \details Draws the graphic to scale on the specified bitmap */
	virtual void draw_to_scale(const DrawingScaledAttr & attr);

	/*! \details This returns the bounds of the icon.  It is only valid after
	 * the icon has been drawn on a bitmap.
	 *
	 * @return The bounds of the last time this icon was drawn on a bitmap using draw_to_scale()
	 */
	sg_bounds_t & bounds(){ return m_bounds; }

private:
	IconAttr m_attr;
	sg_bounds_t m_bounds;

};

};


#endif /* DRAW_ICON_HPP_ */
