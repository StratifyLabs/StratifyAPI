#include "ux/ProgressBar.hpp"
#include "ux/Rectangle.hpp"


using namespace sgfx;
using namespace ux;


void ProgressBar::draw(const DrawingAttributes & attributes){

   DrawingArea active_area(
            value() * (1000 - m_border_thickness*2) / maximum(),
            1000 - m_border_thickness*2
            );

   Rectangle()
         .set_color(color_border)
         .draw(attributes, DrawingPoint(0,0), DrawingArea(1000,1000));

   printf("Draw progress active area %d\n", active_area.width());
   Rectangle()
         .set_color(color_text)
         .draw(
            attributes,
            DrawingPoint(m_border_thickness,m_border_thickness),
            active_area
            );
}

