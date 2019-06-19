/* Copyright 2014-2016 Tyler Gilbert, Inc; All Rights Reserved
 *
 */

#ifndef UI_TIMETAB_HPP_
#define UI_TIMETAB_HPP_

#include "../draw/Drawing.hpp"
#include "../var/String.hpp"
#include "Tab.hpp"

namespace ui {

/*! \brief Tab Time Class
 * \details This class shows a Tab with the time on it.
 */
class TimeTab : public Tab {
public:
	/*! \details Construct a TabTime object */
	TimeTab();
	void draw(const draw::DrawingAttributes & attr);

};

}

#endif /* UI_TIMETAB_HPP_ */
