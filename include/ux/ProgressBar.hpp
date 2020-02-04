/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see LICENSE.md for rights.
#ifndef SAPI_UX_PROGRESSBAR_HPP
#define SAPI_UX_PROGRESSBAR_HPP

#include "Progress.hpp"


namespace ux {

class ProgressBar : public Progress {
public:
   ProgressBar & set_thickness(drawing_size_t thickness);
   Progress& set_border_thickness(drawing_size_t value){
      m_border_thickness = value;
      return *this;
   }

   drawing_size_t border_thickness() const {
      return m_border_thickness;
   }

   void draw_to_scale(const DrawingScaledAttributes & attributes);

private:
   drawing_size_t m_border_thickness = 100;

};

}

#endif // PROGRESSBAR_HPP
