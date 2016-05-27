//Copyright 2011-2016 Tyler Gilbert; All Rights Reserved

#include "draw.hpp"
#include "ui/TabLabel.hpp"
using namespace ui;

TabLabel::TabLabel(){}

void TabLabel::draw(const DrawingAttr & attr){
	label().draw(attr);
}
