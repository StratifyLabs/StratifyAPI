/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see LICENSE.md for rights.

#ifndef SAPI_UX_ICON_HPP_
#define SAPI_UX_ICON_HPP_


#include "Drawing.hpp"
#include "../sgfx/Vector.hpp"
#include "../var/String.hpp"

namespace ux {

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

class Icon : public Drawing, public DrawingAlignment<Icon> {
public:
	/*! \details Construct an empty graphic */
	Icon();

	/*! \details Icon rotation orientations */
	enum rotation {
		rotation_right /*! \brief Point to the right */ = 0,
		rotation_down /*! \brief Point down */ = SG_TRIG_POINTS/4,
		rotation_left /*! \brief Point to the left */ = SG_TRIG_POINTS/2,
		rotation_up /*! \brief Point up */ = SG_TRIG_POINTS*3/4,
		rotation_quarter_clockwise /*! \brief Add/subtract to/from RIGHT, DOWN, etc */ = SG_TRIG_POINTS/4,
		rotation_quarter_counter_clockwise /*! \brief Add/subtract to/from RIGHT, DOWN, etc */ = -SG_TRIG_POINTS/4,
		rotation_eighth_clockwise /*! \brief Add/subtract to/from RIGHT, DOWN, etc */  = SG_TRIG_POINTS/8,
		rotation_eighth_counter_clockwise /*! \brief Add/subtract to/from RIGHT, DOWN, etc */  = -SG_TRIG_POINTS/8,
	};

	const var::String & icon() const { return m_icon; }
	Icon & set_icon(const var::String & value){
		m_icon = value;
		return *this;
	}

	Icon & set_color(sg_color_t value){
		m_color = value;
		return *this;
	}

	/*! \details Set the rotation */
	Icon & set_rotation(s16 rotation){ m_rotation = rotation; return *this; }

	/*! \details Returns the rotation */
	s16 rotation() const { return m_rotation; }

	/*! \details Draws the graphic to scale on the specified bitmap */
	virtual void draw_to_scale(const DrawingScaledAttributes & attr);

	/*! \details This returns the bounds of the icon.  It is only valid after
	 * the icon has been drawn on a bitmap.
	 *
	 * @return The bounds of the last time this icon was drawn on a bitmap using draw_to_scale()
	 */
	sgfx::Region bounds() const { return m_bounds; }

private:
	/*! \cond */
	sg_region_t m_bounds;
	var::String m_icon;
	s16 m_rotation;
	sg_color_t m_color;
	/*! \endcond */

};

}


#endif /* SAPI_UX_ICON_HPP_ */
