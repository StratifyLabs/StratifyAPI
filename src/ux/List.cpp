#include "ux/List.hpp"
#include "ux/Rectangle.hpp"
#include "ux/Text.hpp"
#include "ux/Icon.hpp"
#include "sys/Printer.hpp"

using namespace sgfx;
using namespace ux;

void ListItem::draw_to_scale(const DrawingScaledAttributes & attributes){

   //draw the key on the left
   sg_size_t border_size = attributes.height() * list()->border_size() / 100;
   if( !border_size && list()->border_size() ){
      border_size = 1;
   }
   const sg_size_t padding = attributes.height() / 10;
   const sg_size_t effective_height = attributes.height() - padding*2;
   const Point icon_point(
            attributes.width() - effective_height - padding*2,
            padding
            );

   const Area icon_area(
            effective_height,
            effective_height
            );

   //save the region the item is drawn in
   m_region = attributes.region();

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
#if 1
      Icon().set_icon(m_icon)
            .set_color(Component::color_text)
            .draw_to_scale(attributes +
                           icon_point +
                           icon_area
                           );
      text_offset = icon_area.width() + padding;
#endif
   }

   if( m_value.is_empty() == false ){
      printf("Draw value %s\n", value().cstring());
      Text().set_string(value())
            .set_color(Component::color_text)
            .set_align_right()
            .set_align_middle()
            .draw_to_scale(
               attributes +
               Point(padding, padding) +
               Area(attributes.width() - text_offset - padding*2, effective_height)
               );
   }
}

void List::draw_to_scale(const DrawingScaledAttributes & attributes){
   //draw each list item in the list
   sg_size_t item_height = m_item_height * attributes.height() / 1000;

   Area list_item_area(
            attributes.width(),
            item_height
            );

   sys::Printer p;

   p.info("Draw list item height %d and %d -> %d", m_item_height, item_height);
   p.open_object("attributes");
   p << attributes.point();
   p << attributes.area();
   p.close_object();

   for(size_t i = 0; i < m_items.count(); i++){
      DrawingScaledAttributes item_attributes =
            attributes +
            Point(0,i*item_height) +
            list_item_area;
      p.open_object("item attributes");
      p << item_attributes.point();
      p << item_attributes.area();
      p.close_object();
      m_items.at(i).draw_to_scale(
               item_attributes
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


      if( touch_event.id() == ux::TouchEvent::id_released ){
         set_theme_state(Theme::state_default);
         set_refresh_window(sgfx::Region());
         set_refresh_drawing_pending();
      }

      if( touch_event.id() == ux::TouchEvent::id_pressed ){

         for(const auto & item: m_items){
            if( item.region().contains(
                   touch_event.point()
                   ) ){
               set_refresh_window( item.region() );
               set_theme_state(Theme::state_highlighted);
               set_refresh_drawing_pending();
               break;
            }
         }


         set_theme_state(Theme::state_highlighted);
         refresh_drawing();
      }
   }

   Component::handle_event(event);
}
