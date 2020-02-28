#include "ux/Graph.hpp"

using namespace sgfx;
using namespace ux;



sgfx::Point LineGraph::transform_point(
		const DrawingScaledAttributes & drawing_attributes,
		const LineGraphPoint & line_graph_point
		){
	return Point(
				(line_graph_point.x() - x_axis().minimum()) *
				drawing_attributes.bitmap().width() / x_axis().range(),
				(line_graph_point.y() - y_axis().minimum()) *
				drawing_attributes.bitmap().width() / y_axis().range()
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

	//draw the lines
	Point cursor;

	bool is_first_point = true;
	sg_color_t color = theme()->color();
	for(const auto & data: data_set()){
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

void BarGraph::draw_to_scale(
		const DrawingScaledAttributes & attributes
		){

}
