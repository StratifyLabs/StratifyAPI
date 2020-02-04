/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see LICENSE.md for rights.
#include "ux/ToggleSwitch.hpp"
#include "ux/Rectangle.hpp"
#include "ux/Scene.hpp"

using namespace ux;
using namespace sgfx;

void ToggleSwitch::draw_to_scale(const DrawingScaledAttributes & attributes){

   const sg_size_t border_offset = attributes.height() / 100 ? attributes.height()/100 : 1;
   const sg_size_t active_area_offset = attributes.height() / 10;
   const sg_size_t toggle_area_offset = active_area_offset + attributes.height()/10;
   const sg_size_t toggle_area_total_width = attributes.width() - toggle_area_offset*2;
   const sg_size_t toggle_area_active_width = toggle_area_total_width * 60 / 100;
   const sg_size_t toggle_travel = toggle_area_total_width - toggle_area_active_width;

   const Area toggle_area(
            toggle_area_active_width,
            attributes.height() - toggle_area_offset*2
            );

   //border
   attributes.bitmap() << Pen().set_color(color_border);
   attributes.bitmap().draw_rectangle(
            attributes.region()
            );

   //default
   attributes.bitmap() << Pen().set_color(color_default);
   attributes.bitmap().draw_rectangle(
            attributes.point() + Point(border_offset, border_offset),
            attributes.area() - Area(border_offset*2, border_offset*2)
            );

   //active background
   if( state() == true ){
      attributes.bitmap() << Pen().set_color(color_text);
   } else {
      attributes.bitmap() << Pen().set_color(color_border);
   }
   attributes.bitmap().draw_rectangle(
            attributes.point() + Point(active_area_offset, active_area_offset),
            attributes.area() - Area(active_area_offset*2, active_area_offset*2)
            );


   sg_size_t toggle_point_location = 0;
   if( state() == true ){
      toggle_point_location = toggle_travel;
   }

   //default
   attributes.bitmap() << Pen().set_color(color_default);
   attributes.bitmap().draw_rectangle(
            attributes.point() + Point(toggle_area_offset + toggle_point_location, toggle_area_offset),
            toggle_area
            );

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
