/*! \file */ //Copyright 2011-2016 Tyler Gilbert; All Rights Reserved

#ifndef DRAW_PANEL_HPP_
#define DRAW_PANEL_HPP_

#include "Drawing.hpp"

namespace draw {

/*! \brief Panel Class
 * \ingroup element
 * \details This class draws a rounded rectangle for a panel to hold text.
 */
class Panel : public Drawing {
public:
	Panel();
	void draw_to_scale(const DrawingScaledAttr & attr);


};

};

#endif /* DRAW_PANEL_HPP_ */
