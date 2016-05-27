/* Copyright 2014-2016 Fogo Digital, Inc; All Rights Reserved
 *
 */

#ifndef UI_LISTATTR_HPP_
#define UI_LISTATTR_HPP_

#include <mcu/types.h>


namespace ui {

typedef u8 list_attr_size_t;

class ListAttr {
public:
	ListAttr();

	list_attr_size_t selected() const { return m_selected; }
	list_attr_size_t visible_items() const { return m_visible_items; }
	void set_visible_items(list_attr_size_t v);

	void set_selected(list_attr_size_t);
	void inc_selected(void);
	void dec_selected(void);
	list_attr_size_t calc_visible_offset(void) const;
	list_attr_size_t calc_display_items() const;
	list_attr_size_t calc_next_visible(i8 dir) const;


	virtual list_attr_size_t size() const = 0;

private:
	list_attr_size_t m_selected;
	list_attr_size_t m_visible_items;
};

};

#endif /* UI_LISTATTR_HPP_ */
