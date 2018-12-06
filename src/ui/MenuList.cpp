/* Copyright 2014-2016 Fogo Digital, Inc; All Rights Reserved
 *
 */


#include "ui/MenuList.hpp"
using namespace ui;

MenuList::MenuList(LinkedElement * parent) : List(parent){}



list_attr_size_t MenuList::size() const {
	return 1;
}
