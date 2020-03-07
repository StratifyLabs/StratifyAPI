/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see LICENSE.md for rights.
#ifndef SAPI_UX_TOGGLESWITCH_HPP
#define SAPI_UX_TOGGLESWITCH_HPP


#include "Event.hpp"
#include "Component.hpp"
#include "TouchGesture.hpp"

namespace ux {

class ToggleSwitch;

class ToggleSwitchEvent :
		public EventObject<ToggleSwitch, EVENT_TYPE('_','t','o','g')>
{
public:

	 ToggleSwitchEvent(ToggleSwitch& toggle_switch) :
			EventObject(0, &toggle_switch){

   }


};

class ToggleSwitch : public ComponentAccess<
		ToggleSwitch, COMPONENT_SIGNATURE('t','o','g','g')
		> {
public:

	ToggleSwitch(const var::String & name) : ComponentAccess(name){

	}

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
