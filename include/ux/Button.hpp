#ifndef SAPI_UX_BUTTON_HPP
#define SAPI_UX_BUTTON_HPP


#include "Component.hpp"
#include "Icon.hpp"

namespace ux {

class ButtonEvent : public EventObject<EVENT_TYPE('_','b','t','n')> {
public:

   ButtonEvent(
            const var::String name,
            bool state) :
      EventObject<EVENT_TYPE('_','b','t','n')>(0),
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

class Button : public Component {
public:

   bool state() const {
      return m_state;
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

   void draw(const DrawingAttributes & attributes);
   void handle_event(const ux::Event & event);

private:
   var::String m_label;
   var::String m_icon_name;
   s16 m_icon_rotation;
   bool m_state;



};

}

#endif // SAPI_UX_BUTTON_HPP
