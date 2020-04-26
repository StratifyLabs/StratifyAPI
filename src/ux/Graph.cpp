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
		const Region & plot_region,
		const LineGraphPoint & line_graph_point
		){

	const sg_size_t height = plot_region.height() - 1;
	const sg_size_t width = plot_region.width() - 1;


	return Point(
				plot_region.point().x() +
				(line_graph_point.x() - x_axis().minimum()) *
				width / x_axis().range(),
				plot_region.point().y() +
				(height -
				 (line_graph_point.y() - y_axis().minimum()) *
				 height / y_axis().range()
				 )
				);
}

void LineGraph::draw(
		const DrawingScaledAttributes & attributes
		){

	sgfx::Bitmap & bitmap = attributes.bitmap();

	bitmap << sgfx::Pen().set_color(theme()->background_color());
	bitmap.draw_rectangle(attributes.region());

	const sgfx::Region region_inside_margin = calculate_region_inside_margin(
				attributes.region()
				);

	const sgfx::Area effective_axes_area = sgfx::Area(
				attributes.calculate_width( axes_area().width() ),
				attributes.calculate_width( axes_area().height() )
				);

	const sgfx::Region plot_region =
			calculate_region_inside(
				region_inside_margin,
	{ 0,
		0,
		x_axis().is_valid() ? static_cast<u8>(effective_axes_area.height()) : static_cast<u8>(0),
		y_axis().is_valid() ? static_cast<u8>(effective_axes_area.width()) : static_cast<u8>(0) }
				);

	//set the axis color
	if( y_axis().is_valid() ){
		//draw the y axis
		bitmap.draw_line(
					plot_region.point(),
					plot_region.point() + Point::Y(plot_region.height())
					);
	}

	if( x_axis().is_valid() ){

		bitmap.draw_line(
					plot_region.point() + Point::Y(plot_region.height()),
					plot_region.point() + Point(
						plot_region.width(),
						plot_region.height()
						)
					);
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
							plot_region,
							point
							);
			} else {
				Point next_point = transform_point(
							plot_region,
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

void BarGraph::draw(
		const DrawingScaledAttributes & attributes
		){

}
