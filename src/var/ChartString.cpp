/*! \file */ //Copyright 2011-2017 Tyler Gilbert; All Rights Reserved


#include "var/ChartString.hpp"

namespace var {

ChartString::ChartString(){}

void ChartString::set_point(int x, int y){
	append_object(point_key());
	append_number(x_key(), x);
	append_number(y_key(), y);
	end_object();
	end();
}

void ChartString::set_pointf(float x, float y){
	append_object(point_key());
	append_float(x_key(), x);
	append_float(y_key(), y);
	end_object();
	end();
}

void ChartString::start_points(){
	append_array(points_key());
}

void ChartString::end_points(){
	end_array();
	end();
}

void ChartString::append_point(int x, int y){
	append_object();
	append_number(x_key(), x);
	append_number(y_key(), y);
	end_object();
}

void ChartString::append_pointf(float x, float y){
	append_object();
	append_float(x_key(), x);
	append_float(y_key(), y);
	end_object();
}

} /* namespace var */
