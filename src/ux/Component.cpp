#include "ux/Component.hpp"

using namespace ux;

Component::Component(){

}

Component & Component::enable(){
   m_bitmap.allocate(
            m_drawing_attributes.calculate_area_on_bitmap(),
            sgfx::Bitmap::BitsPerPixel(m_drawing_attributes.bitmap().bits_per_pixel())
            );

   m_drawing_attributes.set_bitmap(m_bitmap);
   return *this;
}


Component & Component::disable(){
   m_bitmap.free();
   return *this;
}

void Component::refresh_drawing(){


   //use the palette if it is available

   //using the attributes and bitmap to write the display device

}
