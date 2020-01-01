#ifndef SAPI_UX_COMPONENT_HPP
#define SAPI_UX_COMPONENT_HPP

#include "../api/WorkObject.hpp"
#include "../hal/Display.hpp"
#include "../var/String.hpp"
#include "Drawing.hpp"
#include "Event.hpp"

namespace ux {

class Scene;

class Component : public Drawing {
public:

   using EventHandlerFunction = std::function<void(Component * object, const Event & event)>;
   virtual ~Component(){}

   Component & set_event_handler(EventHandlerFunction event_handler){
      m_event_handler = event_handler;
      return *this;
   }

   Component & set_drawing_area(const DrawingArea & drawing_area){
      m_reference_drawing_attributes.set_area(drawing_area);
      return *this;
   }

   Component & set_drawing_point(const DrawingPoint & drawing_point){
      m_reference_drawing_attributes.set_point(drawing_point);
      return *this;
   }

   Component & enable(
         hal::Display & display
         );
   Component & disable();

   bool is_enabled() const {
      return (m_local_bitmap.to_const_void() != nullptr);
   }

   //update the location of the component (allow animations)

   virtual void handle_event(const ux::Event & event){
      if( m_event_handler ){
         m_event_handler(this, event);
      }
   }

   const var::String & name() const {
      return m_name;
   }

   void redraw(){
      draw(drawing_attributes());
      refresh_drawing();
   }

   sgfx::Region region() const {
      return m_reference_drawing_attributes.calculate_region_on_bitmap();
   }

   bool contains(const sgfx::Point & point){
      return sgfx::Region(
               m_reference_drawing_attributes.calculate_region_on_bitmap()
               ).contains(point);
   }

   Scene * scene(){ return m_scene; }
   const Scene * scene() const { return m_scene; }

protected:
   void refresh_drawing();
   friend class Scene;
   const DrawingAttributes & drawing_attributes() const {
      return m_local_drawing_attributes;
   }

private:

   var::String m_name;
   u32 m_type_id;
   //needs to know where on the display it is drawn
   DrawingAttributes m_reference_drawing_attributes;
   DrawingAttributes m_local_drawing_attributes;
   sgfx::Bitmap m_local_bitmap;
   hal::Display * m_display = nullptr;

   //needs a palette to use while drawing
   sgfx::Palette * m_palette = nullptr;
   Scene * m_scene = nullptr;
   EventHandlerFunction m_event_handler;

   void set_name(const var::String & name){
      m_name = name;
   }

   void set_scene(Scene * scene){
      m_scene = scene;
   }
};


}

#endif // SAPI_UX_COMPONENT_HPP
