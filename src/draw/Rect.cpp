/*! \file */ //Copyright 2011-2018 Tyler Gilbert; All Rights Reserved

#include "draw/Rect.hpp"
using namespace draw;

Rect::Rect(){

}
void Rect::draw_to_scale(const DrawingScaledAttr & attr){
	attr.bitmap().draw_rectangle(attr.point(), attr.area());
}
