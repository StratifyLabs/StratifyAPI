#ifndef SAPI_UX_COMPONENT_HPP
#define SAPI_UX_COMPONENT_HPP

#include "../api/WorkObject.hpp"
#include "../hal/Display.hpp"
#include "../var/String.hpp"
#include "../sgfx/Theme.hpp"
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

   Component & set_theme_style(enum sgfx::Theme::style value){
      m_theme_style = value;
      return *this;
   }

   Component & set_theme_state(enum sgfx::Theme::state value){
      m_theme_state = value;
      return *this;
   }

   enum sgfx::Theme::style theme_style() const {
      return m_theme_style;
   }

   enum sgfx::Theme::state theme_state() const {
      return m_theme_state;
   }

   bool is_antialias() const {
      return m_is_antialias;
   }

   Component & set_antialias(bool value = true){
      m_is_antialias = value;
      return *this;
   }

   Component & enable(
         hal::Display & display
         );
   Component & disable();

   bool is_enabled() const {
      return (m_local_bitmap.to_const_void() != nullptr);
   }

   const sgfx::Theme & theme() const;

   //update the location of the component (allow animations)

   virtual void handle_event(const ux::Event & event){
      if( m_event_handler ){
         m_event_handler(this, event);
      }
   }

   const var::String & name() const {
      return m_name;
   }

   void apply_antialias_filter(const DrawingAttributes & attributes);
   void apply_antialias_filter(const DrawingScaledAttributes & attributes);

   void redraw(){
      draw(drawing_attributes());
      set_refresh_drawing_pending();
   }

   sgfx::Region region() const {
      return m_reference_drawing_attributes.calculate_region_on_bitmap();
   }

   bool contains(const sgfx::Point & point){
      return sgfx::Region(
               m_reference_drawing_attributes.calculate_region_on_bitmap()
               ).contains(point);
   }

   DrawingPoint translate_point(const sgfx::Point & point);

   Scene * scene(){ return m_scene; }
   const Scene * scene() const { return m_scene; }



protected:
   void set_refresh_drawing_pending(){
      m_is_refresh_drawing_pending = true;
   }

   bool is_refresh_drawing_pending() const {
      return m_is_refresh_drawing_pending;
   }

   void refresh_drawing();
   friend class Scene;
   const DrawingAttributes & drawing_attributes() const {
      return m_local_drawing_attributes;
   }

   enum colors {
      color_background = 0,
      color_border = 5,
      color_default = 10,
      color_text = 15
   };

private:

   var::String m_name;
   u32 m_type_id;
   //needs to know where on the display it is drawn
   DrawingAttributes m_reference_drawing_attributes;
   DrawingAttributes m_local_drawing_attributes;
   sgfx::Bitmap m_local_bitmap;
   hal::Display * m_display = nullptr;
   enum sgfx::Theme::style m_theme_style = sgfx::Theme::style_brand_primary;
   enum sgfx::Theme::state m_theme_state = sgfx::Theme::state_default;
   bool m_is_antialias = true;
   bool m_is_refresh_drawing_pending;

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
