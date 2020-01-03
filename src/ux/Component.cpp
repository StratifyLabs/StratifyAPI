#include "ux/Component.hpp"
#include "sys/Printer.hpp"
#include "ux/Scene.hpp"

using namespace ux;


Component & Component::enable(
      hal::Display & display
      ){

   m_reference_drawing_attributes.set_bitmap(display);
   m_display = &display;

   //local bitmap is a small section of the reference bitmap
   m_local_bitmap.allocate(
            m_reference_drawing_attributes.calculate_area_on_bitmap(),
            sgfx::Bitmap::BitsPerPixel(
               m_reference_drawing_attributes.bitmap().bits_per_pixel()
               )
            );

   //local attributes fill local bitmap
   m_local_drawing_attributes
         .set_area(DrawingArea(1000,1000))
         .set_bitmap(m_local_bitmap);

   return *this;
}


Component & Component::disable(){
   m_local_bitmap.free();
   return *this;
}

void Component::refresh_drawing(){
   if( m_display ){
      //use the palette if it is available

      printf("load theme style, state %d, %d\n", m_theme_style, m_theme_state);

      if( scene()->scene_collection()->theme().set_display_palette(
               *m_display,
               m_theme_style,
               m_theme_state
               ) < 0 ){
         printf("--failed to set display palette\n");
      }

      if( m_display->set_window(
               m_reference_drawing_attributes.calculate_region_on_bitmap()
               ) < 0 ){

      }
      //using the attributes and bitmap to write the display device
      m_display->write(m_local_bitmap);

   }
}
