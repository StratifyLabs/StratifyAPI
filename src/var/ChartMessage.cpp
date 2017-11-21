/*! \file */ //Copyright 2011-2017 Tyler Gilbert; All Rights Reserved


#include "var/ChartMessage.hpp"

namespace var {

ChartMessage::ChartMessage(const hal::Dev & device, int max_capacity) :
		Message(device, max_capacity){}

int ChartMessage::create(){
	int ret = 0;
	ret = Message::create();
	if( ret >= 0 ){
		ret |= open_obj(""); //create root
		ret |= write("type", "chart"); //assign type
	}
	return ret;
}

int ChartMessage::write_point(int x, int y){
	int ret = 0;
	ret |= open_obj("point");
	ret |= write("x", (s32)x);
	ret |= write("y", (s32)y);
	ret |= close_obj();
	return ret;
}

int ChartMessage::write_point(float x, float y){
	int ret = 0;
	ret |= open_obj("point");
	ret |= write("x", x);
	ret |= write("y", y);
	ret |= close_obj();
	return ret;
}

int ChartMessage::write_xy(const int * x, const int * y, int samples){
	int i;

	if( state() != CREATE_STATE ){
		return -1;
	}

	if( open_array("xy") < 0 ){
		return -1;
	}

	for(i=0; i < samples; i++){
		if( write_point(x[i], y[i]) < 0 ){
			return -1;
		}
	}

	if( close_array() < 0 ){
		return -1;
	}

	return 0;
}

int ChartMessage::read_xy(int * x, int * y, int samples){
	String access;
	int i;
	if( state() != READ_STATE ){
		return -1;
	}
	get_error(); //clear the error
	for(i=0; i < samples; i++){
		access.sprintf("xy[%d].x", i);
		x[i] = read_num(access.c_str());
		access.sprintf("xy[%d].y", i);
		y[i] = read_num(access.c_str());
		if( get_error() != 0 ){
			break;
		}
	}
	return i;
}

int ChartMessage::write_xy(const float * x, const float * y, int samples){
	int i;
	if( state() != CREATE_STATE ){
		return -1;
	}
	if( open_array("xy") < 0 ){
		return -1;
	}
	for(i=0; i < samples; i++){
		if( write_point(x[i], y[i]) < 0 ){
			return -1;
		}
	}
	if( close_array() < 0 ){
		return -1;
	}
	return 0;
}

int ChartMessage::read_xy(float * x, float * y, int samples){
	String access;
	int i;
	if( state() != READ_STATE ){
		return -1;
	}
	get_error(); //clear the error
	for(i=0; i < samples; i++){
		access.sprintf("xy[%d].x", i);
		x[i] = read_float(access.c_str());
		access.sprintf("xy[%d].y", i);
		y[i] = read_float(access.c_str());
		if( get_error() != 0 ){
			break;
		}
	}
	return i;
}

}
