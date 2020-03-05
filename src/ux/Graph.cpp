#include <cmath>
#include "ux/Graph.hpp"

using namespace sgfx;
using namespace ux;

float GraphAxes::calculate_range_order_of_magnitude(float range){
	return powf(10.f, floorf( log10f(range) ));
}

float GraphAxes::get_minimum_axis_value(float value, float range){
	float range_order_of_magnitude = calculate_range_order_of_magnitude(range);
	return floorf( value / range_order_of_magnitude ) *
			range_order_of_magnitude - range_order_of_magnitude/10.f;
}

float GraphAxes::get_maximum_axis_value(float value, float range){
	float range_order_of_magnitude = calculate_range_order_of_magnitude(range);
	return ceilf( value / range_order_of_magnitude ) *
			range_order_of_magnitude + range_order_of_magnitude/10.f;
}

sgfx::Point LineGraph::transform_point(
		const DrawingScaledAttributes & drawing_attributes,
		const LineGraphPoint & line_graph_point
		){

	const sg_size_t height = drawing_attributes.region().height() - 1;
	const sg_size_t width = drawing_attributes.region().width() - 1;


	return Point(
				drawing_attributes.point().x() +
				(line_graph_point.x() - x_axis().minimum()) *
				width / x_axis().range(),
				drawing_attributes.point().y() +
				(height -
				 (line_graph_point.y() - y_axis().minimum()) *
				 height / y_axis().range()
				 )
				);
}

void LineGraph::draw_to_scale(
		const DrawingScaledAttributes & attributes
		){

	sgfx::Bitmap & bitmap = attributes.bitmap();

	bitmap << sgfx::Pen().set_color(theme()->background_color());
	bitmap.draw_rectangle(
				attributes.region()
				);

	const sgfx::Area offset_area(
				attributes.calculate_width( axes_area().width() ),
				attributes.calculate_width( axes_area().height() )
				);

	//set the axis color
	if( y_axis().is_valid() ){
		//draw the axis lines
		const sg_int_t x_value = offset_area.width();
		const Point start(x_value, 0);
		Point end;
		if( x_axis().is_valid() ){
			end = Point(x_value, bitmap.height() - offset_area.height() );
		} else {
			end = Point(x_value, 0);
		}
		bitmap.draw_line(start, end);
	}

	if( x_axis().is_valid() ){
		Point start;
		Point end;
		const sg_int_t y_value = bitmap.height() - offset_area.height();
		if( y_axis().is_valid() ){
			start = Point(offset_area.width(), y_value);
		} else {
			start = Point(0, y_value);
		}

		if( y2_axis().is_valid() ){
			end = Point(bitmap.width() - offset_area.width(), y_value);
		} else {
			end = Point(bitmap.width(), y_value);
		}

		bitmap.draw_line(start, end);
	}

	if( data_set() == nullptr ){
		return;
	}

	//draw the lines
	Point cursor;

	bool is_first_point = true;
	sg_color_t color = theme()->color();
	for(const auto & data: *data_set()){
		color++;
		if( color == theme()->color_count() ){
			color = theme()->color();
		}
		bitmap << Pen().set_color(
								color
								);

		for(const auto & point: data){
			//map point to a place on the chart and draw it
			if( is_first_point ){
				is_first_point = false;
				cursor = transform_point(
							attributes,
							point
							);
			} else {
				Point next_point = transform_point(
							attributes,
							point
							);

				bitmap.draw_line(
							cursor, next_point
							);
				cursor = next_point;
			}
		}
	}
}


LineGraphPoint LineGraph::minimum() const {

	if( data_set() == nullptr ){
		return LineGraphPoint();
	}

	float x_threshold = HUGE_VALF;
	float y_threshold = HUGE_VALF;

	for(const auto & series: *data_set()){
		for(const auto & data: series){
			if( data.x() < x_threshold ){
				x_threshold = data.x();
			}
			if( data.y() < y_threshold ){
				y_threshold = data.y();
			}
		}
	}
	return LineGraphPoint(x_threshold, y_threshold);
}


LineGraphPoint LineGraph::maximum() const {
	if( data_set() == nullptr ){
		return LineGraphPoint();
	}

	float x_threshold = -1*HUGE_VALF;
	float y_threshold = -1*HUGE_VALF;

	for(const auto & series: *data_set()){
		for(const auto & data: series){
			if( data.x() > x_threshold ){
				x_threshold = data.x();
			}
			if( data.y() > y_threshold ){
				y_threshold = data.y();
			}
		}
	}
	return LineGraphPoint(x_threshold, y_threshold);
}

LineGraph& LineGraph::fit_axes_to_data(){

	LineGraphPoint min = minimum();
	LineGraphPoint max = maximum();

	const float x_range = max.x() - min.x();
	const float y_range = max.y() - min.y();

	x_axis().set_minimum(
				get_minimum_axis_value(min.x(), x_range)
				);

	x_axis().set_maximum(
				get_maximum_axis_value(max.x(), x_range)
				);

	y_axis().set_minimum(
				get_minimum_axis_value(min.y(), y_range)
				);

	y_axis().set_maximum(
				get_maximum_axis_value(max.y(), y_range)
				);

	if( x_range > 0.0f ){
		x_axis().set_minimum(min.x());
		x_axis().set_maximum(max.x());
	} else {
		x_axis().set_minimum(min.x() - 1.0f);
		x_axis().set_maximum(max.x() + 1.0f);
	}
	if( y_range > 0.0f ){
		y_axis().set_minimum(min.y());
		y_axis().set_maximum(max.y());
	} else {
		y_axis().set_minimum(min.y() - 1.0f);
		y_axis().set_maximum(max.y() + 1.0f);
	}

	x_axis().set_major_tick(
				calculate_range_order_of_magnitude(x_range)
				);

	x_axis().set_minor_tick(
				x_axis().major_tick() / 5.0f
				);

	y_axis().set_major_tick(
				calculate_range_order_of_magnitude(y_range)
				);

	y_axis().set_minor_tick(
				y_axis().major_tick() / 5.0f
				);


	return *this;
}

void BarGraph::draw_to_scale(
		const DrawingScaledAttributes & attributes
		){

}
