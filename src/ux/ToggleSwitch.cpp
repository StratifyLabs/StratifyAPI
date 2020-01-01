#include "ux/ToggleSwitch.hpp"
#include "ux/Rectangle.hpp"

using namespace ux;


void ToggleSwitch::draw(const DrawingAttributes & attributes){

   attributes.bitmap().set_pen(
            sgfx::Pen().set_color( m_primary_color )
            );

   //draw the background
   RoundedRectangle()
         .set_radius(50)
         .draw(attributes, DrawingPoint(0,0), DrawingArea(1000,1000));

   attributes.bitmap().set_pen(
            sgfx::Pen().set_color( m_secondary_color )
            );

   RoundedRectangle()
         .set_radius(50)
         .draw(attributes, DrawingPoint(100,100), DrawingArea(800,800));

   DrawingPoint toggle_point = DrawingPoint(150,150);
   if( state() == true ){
      toggle_point.set_x(400);
   }

   attributes.bitmap().set_pen(
            sgfx::Pen().set_color( m_primary_color )
            );

   RoundedRectangle()
         .set_radius(60)
         .draw(attributes, toggle_point, DrawingArea(450,700));
}

void ToggleSwitch::handle_event(const ux::Event & event){
   //change the state when an event happens in the component
   if( event == SystemEvent(SystemEvent::id_enter) ){
      draw(drawing_attributes());
      refresh_drawing();
   }

   Component::handle_event(event);
}
