/* Copyright 2014-2016 Tyler Gilbert, Inc; All Rights Reserved
 *
 */

#ifndef SAPI_UI_ICONTAB_HPP_
#define SAPI_UI_ICONTAB_HPP_

#include "../draw/Icon.hpp"
#include "Tab.hpp"

namespace ui {

/*! \brief Tab Icon Class
 * \details This class is a Tab that shows an Icon.
 */
class IconTab : public Tab {
public:
	/*! \details Construct an icon tab */
	IconTab();
	virtual void draw(const draw::DrawingAttr & attr);

	/*! \details This method accesses the draw::Icon object that is drawn on the tab */
	draw::Icon & icon(){ return m_icon; }

private:
	draw::Icon m_icon;
};

}

#endif /* SAPI_UI_ICONTAB_HPP_ */
