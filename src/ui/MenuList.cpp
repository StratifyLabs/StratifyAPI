/* Copyright 2014-2016 Fogo Digital, Inc; All Rights Reserved
 *
 */


#include "ui/MenuList.hpp"
using namespace ui;

MenuList::MenuList(ElementLinked * parent) : List(parent), m_back(0, this){}


ElementLinked & MenuList::at(list_attr_u32 i){
	return back();
}

list_attr_u32 MenuList::size() const {
	return 1;
}
