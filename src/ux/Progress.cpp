#include "ux/Progress.hpp"

using namespace ux;

Progress::Progress(){
   m_value = 0;
   m_maximum = 100;
   m_background_color = color_transparent();
}
