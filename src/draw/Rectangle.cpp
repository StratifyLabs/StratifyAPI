/*! \file */ //Copyright 2011-2018 Tyler Gilbert; All Rights Reserved

#include "draw/Rectangle.hpp"
#include "sgfx/Vector.hpp"

using namespace draw;

Rectangle::Rectangle(){
}

void Rectangle::draw_to_scale(const DrawingScaledAttributes & attr){
	attr.bitmap().set_pen( attr.bitmap().pen().set_color( color()) );
	attr.bitmap().draw_rectangle(attr.point(), attr.area());
}

RoundedRectangle::RoundedRectangle(){}

void RoundedRectangle::draw_to_scale(const DrawingScaledAttributes & attr){

	attr.bitmap().set_pen( attr.bitmap().pen().set_color( color()) );

	sg_size_t pixel_radius = (m_radius > 100 ? 100 : m_radius) * (attr.area().width()/2) / 100;

	//top line
	attr.bitmap().draw_line(
				attr.point() +
				sgfx::Point(
					arg::XValue(pixel_radius),
					arg::YValue(0)
					),
				attr.point() +
				sgfx::Point(
					arg::XValue(attr.width() - pixel_radius),
					arg::YValue(0)
					)
				);

	//bottom line
	attr.bitmap().draw_line(
				attr.point() +
				sgfx::Point(
					arg::XValue(pixel_radius),
					arg::YValue(attr.area().height())
					),
				attr.point() +
				sgfx::Point(
					arg::XValue(attr.area().width() - pixel_radius),
					arg::YValue(attr.area().height())
					)
				);

	//draw left
	attr.bitmap().draw_line(
				attr.point() +
				sgfx::Point(
					arg::XValue(0),
					arg::YValue(pixel_radius)
					),
				attr.point() +
				sgfx::Point(
					arg::XValue(0),
					arg::YValue(attr.area().height() - pixel_radius)
					)
				);


	//draw right
	attr.bitmap().draw_line(
				attr.point() +
				sgfx::Point(
					arg::XValue(attr.area().width()),
					arg::YValue(pixel_radius)
					),
				attr.point() +
				sgfx::Point(
					arg::XValue(attr.area().width()),
					arg::YValue(attr.area().height() - pixel_radius)
					)
				);

	//draw corners
	//top left
	attr.bitmap().draw_quadratic_bezier(
				attr.point() +
				sgfx::Point(
					arg::XValue(0),
					arg::YValue(pixel_radius)
					),
				attr.point(),
				attr.point() +
				sgfx::Point(
					arg::XValue(pixel_radius),
					arg::YValue(0)
					)
				);

	//top right
	attr.bitmap().draw_quadratic_bezier(
				attr.point() +
				sgfx::Point(
					arg::XValue(attr.area().width() - pixel_radius),
					arg::YValue(0)
					),
				attr.point() +
				sgfx::Point(
					arg::XValue(attr.area().width()),
					arg::YValue(0)
					),
				attr.point() +
				sgfx::Point(
					arg::XValue(attr.area().width()),
					arg::YValue(pixel_radius)
					)
				);

	//bottom right
	attr.bitmap().draw_quadratic_bezier(
				attr.point() +
				sgfx::Point(
					arg::XValue(attr.area().width()),
					arg::YValue(attr.area().height() - pixel_radius)
					),
				attr.point() +
				sgfx::Point(
					arg::XValue(attr.area().width()),
					arg::YValue(attr.area().height())
					),
				attr.point() +
				sgfx::Point(
					arg::XValue(attr.area().width() - pixel_radius),
					arg::YValue(attr.area().height())
					)
				);

	//bottom left
	attr.bitmap().draw_quadratic_bezier(
				attr.point() +
				sgfx::Point(
					arg::XValue(pixel_radius),
					arg::YValue(attr.area().height())
					),
				attr.point() +
				sgfx::Point(
					arg::XValue(0),
					arg::YValue(attr.area().height())
					),
				attr.point() +
				sgfx::Point(
					arg::XValue(0),
					arg::YValue(attr.area().height()-pixel_radius)
					)
				);

	//now fill
	attr.bitmap().draw_pour(attr.region().center(), attr.region());

}
