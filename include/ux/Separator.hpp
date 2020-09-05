#ifndef SAPI_UX_SEPARATOR_HPP
#define SAPI_UX_SEPARATOR_HPP

#include "Component.hpp"

namespace	ux {



class Separator : public ComponentAccess<Separator>{
public:
  Separator(const var::String &name) : ComponentAccess(name) {}
  void draw(const DrawingScaledAttributes &attributes);
};

}

#endif // SEPARATOR_HPP
