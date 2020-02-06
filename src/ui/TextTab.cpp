/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see LICENSE.md for rights.
//Copyright 2011-2018 Tyler Gilbert; All Rights Reserved

#include "ui/TextTab.hpp"
#include "draw.hpp"
using namespace ui;

TextTab::TextTab(){
	text().set_align_bottom();
}

void TextTab::draw(const DrawingAttr & attr){
	text().draw(attr);
}
