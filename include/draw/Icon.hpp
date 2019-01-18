/*! \file */ //Copyright 2011-2018 Tyler Gilbert; All Rights Reserved

#ifndef DRAW_ICON_HPP_
#define DRAW_ICON_HPP_


#include "Drawing.hpp"
#include "../sgfx/Vector.hpp"
#include "../var/String.hpp"

namespace draw {

/*! \brief Icon Attributes Class
 * \ingroup element
 * \details This class defines the attributes of a Gfx object.
 */

class IconAttributes : public api::DrawInfoObject {
public:

	IconAttributes(const var::ConstString & name = ""){
		set_name(name);
		m_rotation = RIGHT;
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

	const var::String & name() const { return m_name; }
	void set_name(const var::ConstString & name){
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

/*! \brief Icon Class
 * \details This class draws icons that can be scaled and rotated on a bitmap.
 *
 * When an icon is drawn, the icon's pen attributes are used to draw on
 * the bitmap.
 *
 */

class Icon : public Drawing, public IconAttributes {
public:
	/*! \details Construct an empty graphic */
	Icon();

	/*! \details Draws the graphic to scale on the specified bitmap */
	virtual void draw_to_scale(const DrawingScaledAttr & attr);

	/*! \details This returns the bounds of the icon.  It is only valid after
	 * the icon has been drawn on a bitmap.
	 *
	 * @return The bounds of the last time this icon was drawn on a bitmap using draw_to_scale()
	 */
	sg_region_t & bounds(){ return m_bounds; }

private:
	sg_region_t m_bounds;

};

}


#endif /* DRAW_ICON_HPP_ */
