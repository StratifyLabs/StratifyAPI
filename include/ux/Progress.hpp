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

   Progress& set_primary_color(sg_color_t value){
      m_primary_color = value;
      return *this;
   }

   Progress& set_secondary_color(sg_color_t value){
      m_secondary_color = value;
      return *this;
   }

   sg_color_t primary_color() const {
      return m_primary_color;
   }

   sg_color_t secondary_color() const {
      return m_secondary_color;
   }

private:
   u16 m_value;
   u16 m_maximum;
   sg_color_t m_primary_color;
   sg_color_t m_secondary_color;

};

}



#endif // PROGRESS_HPP
