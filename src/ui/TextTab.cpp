//Copyright 2011-2016 Tyler Gilbert; All Rights Reserved

#include "ui/TextTab.hpp"
#include "draw.hpp"
using namespace ui;

TextTab::TextTab(){
	text().set_align_bottom();
}

void TextTab::draw(const DrawingAttr & attr){
	text().draw(attr);
}
