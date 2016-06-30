/* Copyright 2014-2016 Tyler Gilbert, Inc; All Rights Reserved
 *
 */

#ifndef UI_LISTITEMINFO_HPP_
#define UI_LISTITEMINFO_HPP_

#include "../draw/TextAttr.hpp"
#include "../draw/Icon.hpp"
#include "ElementLinked.hpp"
#include "List.hpp"

namespace ui {

class ListItemInfo : public ElementLinked {
public:
	ListItemInfo(ElementLinked * parent = 0);
	ListItemInfo(const char * label, const char * value, ElementLinked * parent = 0);

	void set(const char * l, const char * v);

	virtual Element * handle_event(const Event  & event, const draw::DrawingAttr & attr);

	draw::TextAttr & label(){ return m_label; }
	draw::TextAttr & value(){ return m_value; }

	void draw_to_scale(const draw::DrawingScaledAttr & attr);
private:
	draw::TextAttr m_label;
	draw::TextAttr m_value;

};

}

#endif /* UI_LISTITEMINFO_HPP_ */
