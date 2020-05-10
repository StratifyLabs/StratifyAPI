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

class Icon : public Drawing, public DrawingComponentProperties<Icon> {
public:
	/*! \details Construct an empty graphic */
	Icon();

	Icon & set_name(const var::String & value){
		if( value.find("icon@") == 0 ){
			m_name = value.create_sub_string(
						var::String::Position(5)
						);
		} else {
			m_name = value;
		}
		return *this;
	}


	/*! \details Draws the graphic to scale on the specified bitmap */
	virtual void draw(const DrawingScaledAttributes & attr);

	/*! \details This returns the bounds of the icon.  It is only valid after
	 * the icon has been drawn on a bitmap.
	 *
	 * @return The bounds of the last time this icon was drawn on a bitmap using draw_to_scale()
	 */
	sgfx::Region bounds() const { return m_bounds; }

private:

	API_ACCESS_COMPOUND(Icon,var::String,icon_font_name);
	API_READ_ACCESS_COMPOUND(Icon,var::String,name);
	API_ACCESS_FUNDAMENTAL(Icon,sg_color_t,color,0);
	/*! \cond */
	sg_region_t m_bounds;
	sgfx::IconFont * m_icon_font;
	/*! \endcond */

	sgfx::IconFont * icon_font(){
		return m_icon_font;
	}

	bool resolve_font(sg_size_t h);

};

}


#endif /* SAPI_UX_DRAW_ICON_HPP_ */
