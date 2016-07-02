/* Copyright 2014-2016 Tyler Gilbert, Inc; All Rights Reserved
 *
 */

#ifndef UI_MENULIST_HPP_
#define UI_MENULIST_HPP_


#include "List.hpp"
#include "ListItem.hpp"

namespace ui {

/*! \brief Menu List Class
 * \details This class implements a list with characteristics of a menu.
 * It provides a ui::ListItemBack object to be appended to the end of the list.
 *
 */
class MenuList : public ui::List {
public:

	/*! \details Construct a menu list item */
	MenuList(ui::ElementLinked * parent = 0);

	/*! \details Access the reference to the back entry */
	ui::ListItemBack & back(){ return m_back; }
	virtual ui::ElementLinked * at(ui::list_attr_size_t i);
	virtual ui::list_attr_size_t size() const;

private:
	ui::ListItemBack m_back;

};

};

#endif /* UI_MENULIST_HPP_ */
