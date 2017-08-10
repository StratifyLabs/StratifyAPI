/*! \file */ //Copyright 2011-2017 Tyler Gilbert; All Rights Reserved


#include "draw/Image.hpp"

namespace draw {

Image::Image(){}


void Image::draw_to_scale(const DrawingScaledAttr & attr){


	attr.bitmap().draw_bitmap(attr.point(), *m_bitmap);
}


} /* namespace draw */
