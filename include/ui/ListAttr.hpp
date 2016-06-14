/* Copyright 2014-2016 Fogo Digital, Inc; All Rights Reserved
 *
 */

#ifndef UI_LISTATTR_HPP_
#define UI_LISTATTR_HPP_

#include <mcu/types.h>


namespace ui {

/*! \details Define the type for the items in the list */
typedef u8 list_attr_size_t;

/*! \brief List Attribute Class
 * \details The list attribute stores the data needed
 * to create and navigate a list.
 */
class ListAttr {
public:
	/*! \details Construct a List Attribute object */
	ListAttr();

	/*! \details Return the value of the currently selected item in the list */
	list_attr_size_t selected() const { return m_selected; }

	/*! \details Return the number of visible items in the list */
	list_attr_size_t visible_items() const { return m_visible_items; }

	/*! \details Set the number of visible items in the list */
	void set_visible_items(list_attr_size_t v);

	/*! \details Set which item in the list is currently selected */
	void set_selected(list_attr_size_t);

	/*! \details Increment the currently selected value */
	void inc_selected(void);

	/*! \details Decrement the currently selected value */
	void dec_selected(void);

	/*! \details Calculate the offset to the visible items */
	list_attr_size_t calc_visible_offset(void) const;

	/*! \details Calculate the number of items to display */
	list_attr_size_t calc_display_items() const;

	/*! \details Calculate the index of the next visible item in the specified direction */
	list_attr_size_t calc_next_visible(s8 dir) const;

	/*! \details Calculate the index of the next visible item in the specified direction */
	virtual list_attr_size_t size() const = 0;

private:
	list_attr_size_t m_selected;
	list_attr_size_t m_visible_items;
};

};

#endif /* UI_LISTATTR_HPP_ */
