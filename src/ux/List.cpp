#include "ux/List.hpp"
#include "ux/Rectangle.hpp"
#include "ux/Text.hpp"
#include "ux/Icon.hpp"

using namespace sgfx;
using namespace ux;

void ListItem::draw_to_scale(const DrawingScaledAttributes & attributes){

   //draw the key on the left
   sg_size_t border_size = attributes.height() * list()->border_size() / 100;
   const sg_size_t padding = attributes.height() / 10;
   const sg_size_t effective_height = attributes.height() - padding*2;
   const Point icon_point(
            attributes.width() - attributes.height() - padding*2,
            padding
            );

   const Area icon_area(
            effective_height,
            effective_height
            );

   Rectangle().set_color(Component::color_border)
         .draw_to_scale(attributes);

   Rectangle().set_color(Component::color_default)
         .draw_to_scale(
            attributes +
            Point(border_size, border_size) +
            (attributes.area() - Area(border_size*2, border_size*2))
            );

   Text().set_string(key())
         .set_color(Component::color_text)
         .set_align_left()
         .set_align_middle()
         .draw_to_scale(attributes + Point(padding, padding) + Area(attributes.width(), effective_height));

   sg_int_t text_offset = 0;
   if( m_icon.is_empty() == false ){
      Icon().set_icon(m_icon)
            .set_color(Component::color_text)
            .draw_to_scale(attributes);
      text_offset = icon_area.width() + padding*2;
   }

   if( m_value.is_empty() == false ){
      Text().set_string(value())
            .set_color(Component::color_text)
            .set_align_right()
            .set_align_middle()
            .draw_to_scale(attributes + Point(padding, padding) + Area(attributes.width() - text_offset, effective_height));
   }
}

void List::draw_to_scale(const DrawingScaledAttributes & attributes){
   //draw each list item in the list
   sg_size_t item_height = m_item_height * attributes.height() / 100;

   Area list_item_area(
            attributes.width(),
            item_height
            );

   for(size_t i = 0; i < m_items.count(); i++){
      m_items.at(i).draw_to_scale(
               attributes +
               Point(0,i*item_height) +
               list_item_area
               );
   }

   apply_antialias_filter(attributes);
}

void List::handle_event(const ux::Event & event){
   //change the state when an event happens in the component
   if( event == SystemEvent(SystemEvent::id_enter) ){
      for(auto & item: m_items){
         item.m_list = this;
      }
      redraw();
   } else if( event.type() == ux::TouchEvent::event_type() ){
      const ux::TouchEvent & touch_event
            = event.reinterpret<ux::TouchEvent>();

      if( (touch_event.id() == ux::TouchEvent::id_released) &&
          (contains(touch_event.point()) || theme_state() == Theme::state_highlighted) ){


         set_theme_state(Theme::state_default);
         set_refresh_drawing_pending();
      }

      if( (touch_event.id() == ux::TouchEvent::id_pressed) &&
          contains(touch_event.point()) ){



         set_theme_state(Theme::state_highlighted);
         refresh_drawing();
      }
   }

   Component::handle_event(event);
}
