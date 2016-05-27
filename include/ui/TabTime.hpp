/* Copyright 2014-2016 Fogo Digital, Inc; All Rights Reserved
 *
 */

#ifndef UI_TAB_TABTIME_HPP_
#define UI_TAB_TABTIME_HPP_

#include "../draw/Drawing.hpp"
#include "Tab.hpp"

namespace ui {

class TabTime : public Tab {
public:
	TabTime();
	void draw(const draw::DrawingAttr & attr);

};

};

#endif /* UI_TAB_TABTIME_HPP_ */
