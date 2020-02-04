/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see LICENSE.md for rights.
/* Copyright 2014-2016 Tyler Gilbert, Inc; All Rights Reserved
 *
 */

#ifndef SAPI_UI_MENU_HPP_
#define SAPI_UI_MENU_HPP_

#include "LinkedElement.hpp"

namespace ui {

/*! \brief Menu Class
 * \details This class implements a menu with navigation
 * to the left and right including animations (if supported by the system).
 *
 * You must provide another ui::ElementLinked object to be the starting element.
 * Menu::handle_event() handles these events to navigate between linked elements:
 * - Event::LIST_ACTUATE
 * - Event::MENU_BACK
 *
 *
 */
class Menu : public LinkedElement {
public:

	/*! \details Construct a new Menu object */
	Menu();

	virtual Element * handle_event(const Event & event, const draw::DrawingAttr & attr);
	virtual void draw(const draw::DrawingAttr & attr);

	/*! \details Access a reference to the current element */
	LinkedElement & current(){ return *m_current; }

	/*! \details Set the reference of the current element */
	void set_current(Element & v){ m_current = (LinkedElement*)&v; }

private:

	draw::Animation m_animation;
	LinkedElement * m_current;

};

}

#endif /* SAPI_UI_MENU_HPP_ */
