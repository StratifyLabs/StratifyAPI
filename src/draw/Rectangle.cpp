/*! \file */ //Copyright 2011-2018 Tyler Gilbert; All Rights Reserved

#include "draw/Rectangle.hpp"
#include "sgfx/Vector.hpp"

using namespace draw;

Rectangle::Rectangle(){
}

void Rectangle::draw_to_scale(const DrawingScaledAttributes & attr){
	attr.bitmap().pen().set_color(color());
	attr.bitmap().draw_rectangle(attr.point(), attr.area());
}

RoundedRectangle::RoundedRectangle(){

}

void RoundedRectangle::draw_to_scale(const DrawingScaledAttributes & attr){

	attr.bitmap().pen().set_color(color());

	sg_size_t smallest_dimension =
			attr.area().width() > attr.area().height() ?
				attr.area().height() :
				attr.area().width();

	sg_size_t pixel_radius = (m_radius > 100 ? 100 : m_radius) * (smallest_dimension/2) / 100;

	//draw center rectangle
	attr.bitmap().draw_rectangle(
				attr.point() + sgfx::Point(pixel_radius, 0),
				sgfx::Area(attr.area().width() - pixel_radius*2, attr.area().height())
				);

	//draw left rectangle
	attr.bitmap().draw_rectangle(
				attr.point() + sgfx::Point(0, pixel_radius),
				sgfx::Area(pixel_radius, attr.area().height() - 2*pixel_radius)
				);

	//draw left rectangle
	attr.bitmap().draw_rectangle(
				attr.point() + sgfx::Point(attr.area().width() - pixel_radius, pixel_radius),
				sgfx::Area(pixel_radius, attr.area().height() - 2*pixel_radius)
				);

	//draw corners
	//top left
	attr.bitmap().draw_quadratic_bezier(
				attr.point() +
				sgfx::Point(0, pixel_radius),
				attr.point(),
				attr.point() +
				sgfx::Point(pixel_radius, 0)
				);

	//top right
	attr.bitmap().draw_quadratic_bezier(
				attr.point() +
				sgfx::Point(attr.area().width() - pixel_radius, 0),
				attr.point() +
				sgfx::Point(attr.area().width(), 0),
				attr.point() +
				sgfx::Point(attr.area().width(), pixel_radius)
				);

	//bottom right
	attr.bitmap().draw_quadratic_bezier(
				attr.point() +
				sgfx::Point(attr.area().width(), attr.area().height() - pixel_radius),
				attr.point() +
				sgfx::Point(attr.area().width(), attr.area().height()),
				attr.point() +
				sgfx::Point(attr.area().width() - pixel_radius, attr.area().height())
				);

	//bottom left
	attr.bitmap().draw_quadratic_bezier(
				attr.point() +
				sgfx::Point(pixel_radius, attr.area().height()),
				attr.point() +
				sgfx::Point(0, attr.area().height()),
				attr.point() +
				sgfx::Point(0, attr.area().height()-pixel_radius)
				);

	//now fill corners
	attr.bitmap().draw_pour(
				attr.point() + sgfx::Point(pixel_radius-1, pixel_radius-1),
				attr.region()
				);

	attr.bitmap().draw_pour(
				attr.point() + sgfx::Point(attr.area().width() - pixel_radius + 1, pixel_radius-1),
				attr.region()
				);

	attr.bitmap().draw_pour(
				attr.point() + sgfx::Point(attr.area().width() - pixel_radius + 1, attr.area().height() - pixel_radius + 1),
				attr.region()
				);

	attr.bitmap().draw_pour(
				attr.point() + sgfx::Point(pixel_radius - 1, attr.area().height() - pixel_radius + 1),
				attr.region()
				);

}
