/* Copyright 2014-2016 Tyler Gilbert, Inc; All Rights Reserved
 *
 */

#ifndef UI_MENU_HPP_
#define UI_MENU_HPP_

#include "ElementLinked.hpp"

namespace ui {

class Menu : public ElementLinked {
public:
	Menu();

	virtual Element * handle_event(const Event & event, const draw::DrawingAttr & attr);
	virtual void draw(const draw::DrawingAttr & attr);

	virtual void set_animation_type(u8 v);
	virtual u8 animation_type() const;


	ElementLinked * current(){ return m_current; }
	void set_current(Element * v){ m_current = (ElementLinked*)v; }

private:

	draw::Animation animation;
	ElementLinked * m_current;

};

};

#endif /* UI_MENU_HPP_ */
