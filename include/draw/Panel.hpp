/*! \file */ //Copyright 2011-2018 Tyler Gilbert; All Rights Reserved

#ifndef SAPI_DRAW_PANEL_HPP_
#define SAPI_DRAW_PANEL_HPP_

#include "Drawing.hpp"

namespace draw {


/*! \brief Panel Attributes Class
 *
 */
class PanelAttr : public api::InfoObject {
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

#endif /* SAPI_DRAW_PANEL_HPP_ */
