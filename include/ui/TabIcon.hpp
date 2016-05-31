/* Copyright 2014-2016 Fogo Digital, Inc; All Rights Reserved
 *
 */

#ifndef UI_TAB_TABGFX_HPP_
#define UI_TAB_TABGFX_HPP_

#include "../draw/Icon.hpp"
#include "Tab.hpp"

namespace ui {


class TabIcon : public Tab {
public:
	TabIcon();
	//Gfx & gfx(){ return m_gfx; }
	virtual void draw(const draw::DrawingAttr & attr);

	draw::IconAttr & attr(){ return m_icon.attr(); }

private:
	draw::Icon m_icon;
};

};

#endif /* UI_TAB_TABGFX_HPP_ */
