#include "ux/ProgressBar.hpp"


using namespace sgfx;
using namespace ux;

ProgressBar::ProgressBar(){}


void ProgressBar::draw_to_scale(const DrawingScaledAttributes & attributes){
	//draw the progress bar on the bitmap with x, y at the top left corner
	sg_size_t thickness;
	thickness = (border_thickness() > 100 ? 100 : border_thickness()) *
			(attributes.area().height() / 2) / 100;

	//draw bar
	if( background_color() != color_transparent() ){
		attributes.bitmap() << Pen().set_color( background_color() );
		attributes.bitmap().draw_rectangle(attributes.region());
	}

	//draw progress
	sg_size_t progress_width = attributes.area().width() - thickness*2;

	attributes.bitmap() << Pen().set_color( color() );
	attributes.bitmap().draw_rectangle(
				attributes.point() + Point(thickness, thickness),
				Area(progress_width * value() / maximum(), attributes.area().height() - thickness*2)
				);

}
