/* Copyright 2014-2016 Tyler Gilbert, Inc; All Rights Reserved
 *
 */

#ifndef UI_LISTITEMINFO_HPP_
#define UI_LISTITEMINFO_HPP_

#include "../draw/TextAttr.hpp"
#include "../draw/Icon.hpp"
#include "ElementLinked.hpp"
#include "ListItem.hpp"

namespace ui {

/*! \brief List Item Info Class
 * \details This class is a list item with a label and a value.  Both
 * are display on the same line in a list.
 */
class ListItemInfo : public ListItem {
public:
	ListItemInfo(ElementLinked * parent = 0);
	ListItemInfo(const char * label, const char * value, ElementLinked * parent = 0);

	void set(const char * l, const char * v);

	virtual Element * handle_event(const Event  & event, const draw::DrawingAttr & attr);

	draw::TextAttr & label(){ return text_attr(); }
	draw::TextAttr & value(){ return m_value; }

	void draw_to_scale(const draw::DrawingScaledAttr & attr);
private:
	draw::TextAttr m_value;

};

}

#endif /* UI_LISTITEMINFO_HPP_ */
