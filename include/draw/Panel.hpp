/*! \file */ //Copyright 2011-2016 Tyler Gilbert; All Rights Reserved

#ifndef DRAW_PANEL_HPP_
#define DRAW_PANEL_HPP_

#include "Drawing.hpp"

namespace draw {


/*! \brief Panel Attributes Class
 *
 */
class PanelAttr : public api::DrawInfoObject {
public:
	Pen & pen(){ return m_pen; }
	void set_radius(sg_size_t radius){ m_radius = radius; }
	sg_size_t radius(){ return m_radius; }

protected:
	Pen m_pen;
	sg_size_t m_radius;
};

/*! \brief Panel Class
 * \ingroup element
 * \details This class draws a rounded rectangle for a panel to hold text.
 */
class Panel : public Drawing, public PanelAttr {
public:
	Panel();
	void draw_to_scale(const DrawingScaledAttr & attr);


};

}

#endif /* DRAW_PANEL_HPP_ */
