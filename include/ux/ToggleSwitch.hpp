#ifndef SAPI_UX_TOGGLESWITCH_HPP
#define SAPI_UX_TOGGLESWITCH_HPP


#include "Component.hpp"

namespace ux {

class ToggleSwitch : public Component {
public:
   ToggleSwitch();

   bool state() const {
      return m_state;
   }

   ToggleSwitch& set_state(bool value){
      m_state = value;
      return *this;
   }




   void draw_to_scale(const DrawingScaledAttributes & attributes);
   void handle_event(const ux::Event & event);

private:
   bool m_state;


};

}

#endif // SAPI_UX_TOGGLESWITCH_HPP
