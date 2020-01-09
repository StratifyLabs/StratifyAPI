#include "ux/List.hpp"
#include "ux/Rectangle.hpp"
#include "ux/Text.hpp"

using namespace sgfx;
using namespace ux;

ListItem::ListItem(List * list) : m_list(list){

}


void ListItem::draw_to_scale(const DrawingScaledAttributes & attributes){

   //draw the key on the left
   sg_size_t border_size = attributes.height() * m_border_size / 100;
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
         .draw(attributes);

   Rectangle().set_color(Component::color_border)
         .draw(attributes);

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

}

void List::handle_event(const ux::Event & event){

}
