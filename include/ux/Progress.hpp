/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see
             // LICENSE.md for rights.
#ifndef SAPI_UX_PROGRESS_HPP
#define SAPI_UX_PROGRESS_HPP

#include "Component.hpp"
#include "TouchGesture.hpp"

namespace ux {

class Progress {
public:
private:
  API_AF(Progress, u16, value, 0);
  API_AF(Progress, u16, maximum, 100);
  API_AF(Progress, u16, width, 20);
};

class ProgressBar : public ComponentAccess<ProgressBar> {
public:
  COMPONENT_PREFIX(ProgressBar)

  ProgressBar(const var::String &name) : ComponentAccess(prefix() + name) {
    set_horizontal_padding(10);
    set_vertical_padding(10);
  }

  ProgressBar &set_progress_width(u16 value) {
    progress().set_width(value);
    return *this;
  }

  ProgressBar &set_value(u16 value) {
    progress().set_value(value);
    return *this;
  }

  ProgressBar &set_maximum(u16 value) {
    progress().set_maximum(value);
    return *this;
  }

  void draw(const DrawingScaledAttributes &attributes);

private:
  API_AC(ProgressBar, Progress, progress);
};

} // namespace ux

#endif // SAPI_UX_PROGRESS_HPP
