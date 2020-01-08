#include "ux/Button.hpp"
#include "ux/Rectangle.hpp"
#include "ux/Text.hpp"
#include "ux/Scene.hpp"

using namespace ux;

void Button::draw(const DrawingAttributes & attributes){

   const drawing_size_t border_size = 50; //need to compensate aspect ratio
   const drawing_size_t button_dimension = 1000 - border_size*2;

   //draw the Border
   Rectangle()
         .set_color(color_border)
         .draw(attributes, DrawingPoint(0,0), DrawingArea(1000,1000));

   //draw the button
   Rectangle()
         .set_color(color_default)
         .draw(attributes, DrawingPoint(border_size,border_size), DrawingArea(button_dimension,button_dimension));

   //if the icon is available, draw it

   //if the label is available, draw it
   if( m_label.is_empty() == false ){
      Text().set_string(m_label)
            .set_color(color_text)
            .set_align_center()
            .set_align_middle()
            .draw(attributes, DrawingPoint(0,0), DrawingArea(1000,1000));
   }

   apply_antialias_filter(attributes);

}

void Button::handle_event(const ux::Event & event){
   //change the state when an event happens in the component
   if( event == SystemEvent(SystemEvent::id_enter) ){
      redraw();
   } else if( event.type() == ux::TouchEvent::event_type() ){
      const ux::TouchEvent & touch_event
            = event.reinterpret<ux::TouchEvent>();

      if( (touch_event.id() == ux::TouchEvent::id_released) &&
          contains(touch_event.point()) ){
         toggle();

         if( scene() ){
            scene()->trigger_event(
                     ButtonEvent(name(), state())
                     );
         }


         redraw();
      }
   }

   Component::handle_event(event);
}
