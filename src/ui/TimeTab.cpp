//Copyright 2011-2018 Tyler Gilbert; All Rights Reserved

#include "sys.hpp"
#include "draw.hpp"
#include "chrono.hpp"
#include "ui/TimeTab.hpp"
using namespace ui;

TimeTab::TimeTab(){}

void TimeTab::draw(const DrawingAttributes & attr){
	Time t;
	u32 hour;
	t.get_time_of_day();
	hour = t.hour() % 12;
	if( hour == 0 ){
		hour = 12;
	}
	var::String time_string;
	time_string.format("%ld:%02ld", hour, t.minute());
	Text()
			.set_string(time_string)
			.set_font_point_size(attr.calculate_height_on_bitmap() * 800 / 1000)
			.draw(attr);
}
