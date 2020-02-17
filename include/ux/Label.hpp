/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see LICENSE.md for rights.
#ifndef SAPI_UX_LABEL_HPP
#define SAPI_UX_LABEL_HPP

#include "../sgfx/Font.hpp"
#include "Component.hpp"
#include "Icon.hpp"

namespace ux {

class Label : public Component, public DrawingAlignment<Label> {
public:

   Label& set_border_size(u8 value){
      m_border_size = value;
      return *this;
   }

   Label& set_label(const var::String & value){
      m_label = value;
      return *this;
   }

   Label& set_padding(u8 value){
      m_padding = value;
      return *this;
   }

   Label& set_icon_name(const var::String & value){
      m_icon_name = value;
      return *this;
   }

   void draw_to_scale(const DrawingScaledAttributes & attributes);
	 void handle_event(const ux::Event & event);

private:
   var::String m_label;
   var::String m_icon_name;
   u8 m_border_size = 1;
   u8 m_padding = 10;

};

}

#endif // SAPI_UX_LABEL_HPP
