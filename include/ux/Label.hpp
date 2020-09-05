/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see
             // LICENSE.md for rights.
#ifndef SAPI_UX_LABEL_HPP
#define SAPI_UX_LABEL_HPP

#include "../sgfx/Font.hpp"
#include "Component.hpp"
#include "draw/Icon.hpp"

namespace ux {

class Label : public ComponentAccess<Label> {
public:
  Label(const var::String &name) : ComponentAccess(name) { set_value(name); }

  void draw(const DrawingScaledAttributes &attributes);

private:
  API_ACCESS_COMPOUND(Label, var::String, value);
};

} // namespace ux

#endif // SAPI_UX_LABEL_HPP
