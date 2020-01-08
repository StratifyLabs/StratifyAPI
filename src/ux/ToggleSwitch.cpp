#include "ux/ToggleSwitch.hpp"
#include "ux/Rectangle.hpp"
#include "ux/Scene.hpp"

using namespace ux;

void ToggleSwitch::draw(const DrawingAttributes & attributes){


   //draw the background
   Rectangle()
         .set_color( color_border )
         .draw(attributes, DrawingPoint(0,0), DrawingArea(1000,1000));

   Rectangle()
         .set_color( color_default )
         .draw(attributes, DrawingPoint(0,0), DrawingArea(950,950));

   Rectangle()
         .set_color( color_text )
         .draw(attributes, DrawingPoint(100,100), DrawingArea(800,800));

   DrawingPoint toggle_point = DrawingPoint(150,150);
   if( state() == true ){
      toggle_point.set_x(400);
   }

   Rectangle()
         .set_color( color_default )
         .draw(attributes, toggle_point, DrawingArea(450,700));

   apply_antialias_filter(attributes);

}

void ToggleSwitch::handle_event(const ux::Event & event){
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
                     ToggleSwitchEvent(name(), state())
                     );
         }


         redraw();
      }
   }

   Component::handle_event(event);
}
