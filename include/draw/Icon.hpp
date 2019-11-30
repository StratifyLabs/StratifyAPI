/*! \file */ //Copyright 2011-2018 Tyler Gilbert; All Rights Reserved

#ifndef SAPI_DRAW_ICON_HPP_
#define SAPI_DRAW_ICON_HPP_


#include "Drawing.hpp"
#include "../sgfx/Vector.hpp"
#include "../var/String.hpp"

namespace draw {

/*! \cond */
class IconAttributes : public api::InfoObject {
public:

	IconAttributes(const var::String & name = ""){
		set_name(name);
		m_rotation = RIGHT;
	}

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

	const var::String & name() const { return m_name; }
	void set_name(const var::String & name){
		m_name = name;
	}

	/*! \details Set the rotation */
	void set_rotation(s16 rotation){ m_rotation = rotation; }

	/*! \details Returns the rotation */
	s16 rotation() const { return m_rotation; }

private:
	var::String m_name;
	s16 m_rotation;
};

typedef IconAttributes IconAttr;

/*! \endcond */

/*! \brief Icon Class
 * \details This class draws icons that can be scaled and rotated on a bitmap.
 *
 * When an icon is drawn, the icon's pen attributes are used to draw on
 * the bitmap.
 *
 * The icon is looked up by name using any icon files that are installed
 * in any of the system assets locations.
 *
 *
 */

class Icon : public Drawing {
public:
	/*! \details Construct an empty graphic */
	Icon();

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

	const var::String & name() const { return m_name; }
	Icon & set_name(const var::String & name){
		m_name = name;
		return *this;
	}


	Icon & set_color(sg_color_t value){ Drawing::set_color(value); return *this; }

	Icon & set_align_left(){ Drawing::set_align_left(); return *this; }
	Icon & set_align_right(){ Drawing::set_align_right(); return *this; }
	Icon & set_align_center(){ Drawing::set_align_center(); return *this; }
	Icon & set_align_top(){ Drawing::set_align_top(); return *this; }
	Icon & set_align_middle(){ Drawing::set_align_middle(); return *this; }
	Icon & set_align_bottom(){ Drawing::set_align_bottom(); return *this; }

	/*! \details Set the rotation */
	Icon & set_rotation(s16 rotation){ m_rotation = rotation; return *this; }

	/*! \details Returns the rotation */
	s16 rotation() const { return m_rotation; }

	/*! \details Draws the graphic to scale on the specified bitmap */
	virtual void draw_to_scale(const DrawingScaledAttr & attr);

	/*! \details This returns the bounds of the icon.  It is only valid after
	 * the icon has been drawn on a bitmap.
	 *
	 * @return The bounds of the last time this icon was drawn on a bitmap using draw_to_scale()
	 */
	sgfx::Region bounds() const { return m_bounds; }

private:
	/*! \cond */
	sg_region_t m_bounds;
	var::String m_name;
	s16 m_rotation;
	/*! \endcond */

};

}


#endif /* SAPI_DRAW_ICON_HPP_ */
