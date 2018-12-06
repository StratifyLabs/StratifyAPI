//Copyright 2011-2018 Tyler Gilbert; All Rights Reserved

#include "sys.hpp"
#include "draw.hpp"
#include "chrono.hpp"
#include "ui/TimeTab.hpp"
using namespace ui;

TimeTab::TimeTab(){}

void TimeTab::draw(const DrawingAttr & attr){
	Time t;
	u32 hour;
	t.get_time_of_day();
	hour = t.hour() % 12;
	if( hour == 0 ){
		hour = 12;
	}
	Text label;
	label.string().format("%ld:%02ld", hour, t.minute());
	label.set_font_size( attr.calc_height_on_bitmap() * 800 / 1000 );
	label.draw(attr);
}
