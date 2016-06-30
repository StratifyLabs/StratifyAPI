/* Copyright 2014-2016 Tyler Gilbert, Inc; All Rights Reserved
 *
 */

#ifndef UI_MENULIST_HPP_
#define UI_MENULIST_HPP_


#include "List.hpp"
#include "ListItem.hpp"

namespace ui {


class MenuList : public ui::List {
public:
	MenuList(ui::ElementLinked * parent = 0);

	ui::ListItemBack & back(){ return m_back; }
	virtual ui::ElementLinked * at(ui::list_attr_size_t i);
	virtual ui::list_attr_size_t size() const;

private:
	ui::ListItemBack m_back;

};

};

#endif /* UI_MENULIST_HPP_ */
