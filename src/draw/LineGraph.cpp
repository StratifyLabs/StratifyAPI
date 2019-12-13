/*! \file */ //Copyright 2011-2018 Tyler Gilbert; All Rights Reserved

#include "sgfx.hpp"
#include "draw/LineGraph.hpp"
using namespace draw;

LineGraph::LineGraph(){}

void LineGraph::draw_to_scale(const DrawingScaledAttr & attr){
	//draw the progress bar on the bitmap with x, y at the top left corner

	sgfx::Bitmap & bitmap = attr.bitmap();

	Region active_region = draw_axes(
				attr.bitmap(),
				attr.region()
				);


	//map x axis to width of bitmap

	for(u32 x_index = 0; x_index < x_data().count()-1; x_index++){

		float x0 = x_data().at(x_index);
		float x1 = x_data().at(x_index+1);


		if( (x0 > x_axis().minimum()) && (x0 < x_axis().maximum()) ){

			for(u32 y_series_index = 0;
				 y_series_index < y_data_at().count();
				 y_series_index++){

				const YData & y_data_series = y_data_at(y_series_index);
				bitmap << Pen().set_color( y_data_series.color() );

				if( y_data_series.count() > x_index + 1 ){
					float y0 = y_data_series.at(x_index);
					float y1 = y_data_series.at(x_index+1);

					bitmap.draw_line(
								transform_data_to_point(
									active_region, y_axis_at(y_series_index), x0, y0
									),
								transform_data_to_point(
									active_region, y_axis_at(y_series_index), x1, y1
									)
								);

				}
			}
		}
	}
}
