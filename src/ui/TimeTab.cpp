//Copyright 2011-2018 Tyler Gilbert; All Rights Reserved

#include "sys.hpp"
#include "draw.hpp"
#include "ui/TimeTab.hpp"
using namespace ui;

TimeTab::TimeTab(){}

void TimeTab::draw(const DrawingAttr & attr){
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
	label.set_font_size( attr.calc_height_on_bitmap() * 800 / 1000 );
	label.assign(buffer);
	label.draw(attr);
}
