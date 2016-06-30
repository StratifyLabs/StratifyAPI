/* Copyright 2014-2016 Tyler Gilbert, Inc; All Rights Reserved
 *
 */

#ifndef UI_TAB_TABTIME_HPP_
#define UI_TAB_TABTIME_HPP_

#include "../draw/Drawing.hpp"
#include "Tab.hpp"

namespace ui {

/*! \brief Tab Time Class
 * \details This class shows a Tab with the time on it.
 */
class TabTime : public Tab {
public:
	TabTime();
	void draw(const draw::DrawingAttr & attr);

};

};

#endif /* UI_TAB_TABTIME_HPP_ */
