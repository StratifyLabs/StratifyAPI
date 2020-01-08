#ifndef SAPI_UX_TOGGLESWITCH_HPP
#define SAPI_UX_TOGGLESWITCH_HPP


#include "Event.hpp"
#include "Component.hpp"

namespace ux {

class ToggleSwitchEvent : public EventObject<EVENT_TYPE('_','t','o','g')> {
public:

   ToggleSwitchEvent(
            const var::String name,
            bool state) :
      EventObject<EVENT_TYPE('_','t','o','g')>(0),
      m_name(name),
      m_state(state){

   }

   const var::String & name() const {
      return m_name;
   }

   const bool state() const {
      return m_state;
   }

private:
   const var::String & m_name;
   bool m_state;

};

class ToggleSwitch : public Component {
public:

   bool state() const {
      return m_state;
   }

   ToggleSwitch& set_state(bool value){
      m_state = value;
      return *this;
   }

   ToggleSwitch& toggle(){
      m_state = !m_state;
      return *this;
   }

   void draw_to_scale(const DrawingScaledAttributes & attributes);
   void handle_event(const ux::Event & event);

private:
   bool m_state;

};

}

#endif // SAPI_UX_TOGGLESWITCH_HPP
