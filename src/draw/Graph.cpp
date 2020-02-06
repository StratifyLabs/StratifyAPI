/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see LICENSE.md for rights.

#include "sgfx.hpp"
#include "draw/LineGraph.hpp"
using namespace draw;


Graph::Graph(){}


sgfx::Region Graph::draw_axes(
		sgfx::Bitmap & bitmap,
		const sgfx::Region & region
		){


	return Region();
}


sgfx::Point Graph::transform_data_to_point(
		const sgfx::Region & region,
		const Axis & y_axis,
		float x,
		float y
		){

	sg_point_t point;

	point.x = (x - x_axis().minimum()) / x_axis().range() * region.width() + region.x();
	point.y = (y - y_axis.minimum()) / y_axis.range() * region.height() + region.y();

	return point;
}
