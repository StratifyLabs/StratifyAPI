/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see
             // LICENSE.md for rights.
#ifndef SAPI_UX_TOGGLESWITCH_HPP
#define SAPI_UX_TOGGLESWITCH_HPP

#include "Component.hpp"
#include "Event.hpp"
#include "TouchGesture.hpp"

namespace ux {

class ToggleSwitch;

class ToggleSwitch : public ComponentAccess<ToggleSwitch> {
public:
  enum event_ids { event_id_none, event_id_changed };

  ToggleSwitch(const var::String &name) : ComponentAccess(name) {}

  bool state() const { return m_state; }

  ToggleSwitch &set_state(bool value) {
    m_state = value;
    update_model(value ? "true" : "false");
    return *this;
  }

  ToggleSwitch &toggle() {
    m_state = !m_state;
    update_model(m_state ? "true" : "false");
    return *this;
  }

  void draw(const DrawingScaledAttributes &attributes);
  void handle_event(const ux::Event &event);

private:
  bool m_state;
};

} // namespace ux

#endif // SAPI_UX_TOGGLESWITCH_HPP
