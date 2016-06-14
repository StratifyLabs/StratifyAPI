//Copyright 2011-2016 Tyler Gilbert; All Rights Reserved

#include "sys.hpp"
#include "draw.hpp"
#include "ui/TabTime.hpp"
using namespace ui;

TabTime::TabTime(){}

void TabTime::draw(const DrawingAttr & attr){
	Time t;
	u32 hour;
	t.get_time_of_day();
	char buffer[8];
	hour = t.hour() % 12;
	if( hour == 0 ){
		hour = 12;
	}
	sprintf(buffer, "%ld:%02ld", hour, t.minute());
	Text label;
	label.set_font_size( height_on_bitmap(attr) * 900 / 1000 );
	label.assign(buffer);
	label.draw(attr);
}
