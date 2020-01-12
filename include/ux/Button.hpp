#ifndef SAPI_UX_BUTTON_HPP
#define SAPI_UX_BUTTON_HPP


#include "Component.hpp"
#include "Icon.hpp"

namespace ux {

class ButtonEvent : public EventObject<EVENT_TYPE('_','b','t','n')> {
public:


   enum event_id {
      event_id_pressed,
      event_id_released
   };

   ButtonEvent(
            const var::String name,
            u32 id) :
      EventObject<EVENT_TYPE('_','b','t','n')>(id),
      m_name(name){
   }

	enum button_id {
		id_none,
		id_active,
		id_pressed,
		id_released
	};

   const var::String & name() const {
      return m_name;
   }

private:
   const var::String & m_name;

};

class Button : public Component {
public:

   bool state() const {
      return m_state;
   }

   Button& set_border_size(u8 value){
      m_border_size = value;
      return *this;
   }

   Button& set_state(bool value){
      m_state = value;
      return *this;
   }

   Button& set_label(const var::String & value){
      m_label = value;
      return *this;
   }

   Button& set_icon_name(const var::String & value){
      m_icon_name = value;
      return *this;
   }

   Button& set_icon_rotation(s16 value){
      m_icon_rotation = value;
      return *this;
   }

   Button& toggle(){
      m_state = !m_state;
      return *this;
   }

   void draw_to_scale(const DrawingScaledAttributes & attributes);
   void handle_event(const ux::Event & event);

private:
   var::String m_label;
   var::String m_icon_name;
   s16 m_icon_rotation = 0;
   u8 m_border_size = 1;
   bool m_state;



};

}

#endif // SAPI_UX_BUTTON_HPP
