/* Copyright 2014-2016 Tyler Gilbert, Inc; All Rights Reserved
 *
 */

#ifndef UI_MENU_HPP_
#define UI_MENU_HPP_

#include "ElementLinked.hpp"

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
class Menu : public ElementLinked {
public:

	/*! \details Construct a new Menu object */
	Menu();

	virtual Element * handle_event(const Event & event, const draw::DrawingAttr & attr);
	virtual void draw(const draw::DrawingAttr & attr);

	virtual void set_animation_type(u8 v);
	virtual u8 animation_type() const;


	/*! \details Access a reference to the current element */
	ElementLinked & current(){ return *m_current; }

	/*! \details Set the reference of the current element */
	void set_current(Element & v){ m_current = (ElementLinked*)&v; }

private:

	draw::Animation animation;
	ElementLinked * m_current;

};

};

#endif /* UI_MENU_HPP_ */
