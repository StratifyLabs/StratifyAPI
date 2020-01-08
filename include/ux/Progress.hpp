#ifndef PROGRESS_HPP
#define PROGRESS_HPP


#include "Component.hpp"

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


private:
   u16 m_value;
   u16 m_maximum;

};

}



#endif // PROGRESS_HPP
