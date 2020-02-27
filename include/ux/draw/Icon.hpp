/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see LICENSE.md for rights.

#ifndef SAPI_UX_DRAW_ICON_HPP_
#define SAPI_UX_DRAW_ICON_HPP_


#include "../Drawing.hpp"
#include "../../sgfx/IconFont.hpp"
#include "../../var/String.hpp"

namespace ux::draw {



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

	Icon & set_icon_font_name(const var::String & value){
		m_icon_font_name = value;
		return *this;
	}

	const var::String & name() const { return m_name; }
	Icon & set_name(const var::String & value){
		m_name = value;
		return *this;
	}

	Icon & set_color(sg_color_t value){
		m_color = value;
		return *this;
	}

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
	var::String m_icon_font_name;
	var::String m_name;
	sg_color_t m_color;
	sgfx::IconFont * m_icon_font;
	/*! \endcond */

	sgfx::IconFont * icon_font(){
		return m_icon_font;
	}

	bool resolve_font(sg_size_t h);

};

}


#endif /* SAPI_UX_DRAW_ICON_HPP_ */
