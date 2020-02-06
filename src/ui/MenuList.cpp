/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see LICENSE.md for rights.
/* Copyright 2014-2016 Fogo Digital, Inc; All Rights Reserved
 *
 */


#include "ui/MenuList.hpp"
using namespace ui;

MenuList::MenuList(LinkedElement * parent) : List(parent){}



list_attr_size_t MenuList::size() const {
	return 1;
}
