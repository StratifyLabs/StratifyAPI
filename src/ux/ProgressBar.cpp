/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see LICENSE.md for rights.
#include "ux/ProgressBar.hpp"
#include "ux/draw/Rectangle.hpp"


using namespace sgfx;
using namespace ux;


void ProgressBar::draw_to_scale(const DrawingScaledAttributes & attributes){

   sg_size_t border = 0;
   border = m_border_thickness * attributes.height() / 200;

   sg_size_t progress_size =
         value() * (attributes.area().width() - border * 2) / maximum();


	 attributes.bitmap() << Pen().set_color(theme()->border_color());

   attributes.bitmap().draw_rectangle(
            attributes.point(),
            attributes.area()
            );

	 attributes.bitmap() << Pen().set_color(theme()->text_color());
   attributes.bitmap().draw_rectangle(
            attributes.point() + Point(border, border),
            Area(
               progress_size,
               attributes.area().height() - border*2
               )
            );

   apply_antialias_filter(attributes);

}

