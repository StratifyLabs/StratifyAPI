#ifndef SAPI_UX_PROGRESS_HPP
#define SAPI_UX_PROGRESS_HPP


#include "Component.hpp"
#include "TouchGesture.hpp"

namespace ux {

class Progress : public Component {
public:
   Progress();

   u16 value(){ return m_value; }
   u16 maximum(){ return m_maximum; }

   Progress & set_value(u16 value){
      m_value = value;
      return *this;
   }

   Progress & set_maximum(u16 value){
      m_maximum = value;
      return *this;
   }


   void handle_event(const ux::Event & event);


private:
   u16 m_value;
   u16 m_maximum;

};

}

#endif // SAPI_UX_PROGRESS_HPP
