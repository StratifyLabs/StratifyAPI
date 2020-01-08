#include "ux/Button.hpp"
#include "ux/Rectangle.hpp"
#include "ux/Text.hpp"
#include "ux/Scene.hpp"

using namespace sgfx;
using namespace ux;

void Button::draw_to_scale(const DrawingScaledAttributes & attributes){

   sg_size_t border_size = attributes.width() / 100;
   if( border_size == 0 ){
      border_size = 1;
   }

   //draw the Border
   attributes.bitmap() << Pen().set_color(color_border);
   attributes.bitmap().draw_rectangle(
            attributes.region()
            );

   attributes.bitmap() << Pen().set_color(color_default);
   attributes.bitmap().draw_rectangle(
            attributes.point() + Point(border_size, border_size),
            attributes.area() - Area(border_size, border_size)
            );

   //if the icon is available, draw it

   //if the label is available, draw it
   if( m_label.is_empty() == false ){
      Text().set_string(m_label)
            .set_color(color_text)
            .set_align_center()
            .set_align_middle()
            .draw_to_scale(
               attributes
               );
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
