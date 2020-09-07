/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see
             // LICENSE.md for rights.
#ifndef SAPI_UX_SLIDER_HPP
#define SAPI_UX_SLIDER_HPP

#include "Component.hpp"
#include "EventLoop.hpp"
#include "Model.hpp"

namespace ux {

class Slider;

class Slider : public ComponentAccess<Slider> {
public:
  enum event_ids {
    event_id_none,
    event_id_pressed,
    event_id_active,
    event_id_released,
    event_id_changed
  };

  Slider(const var::String &name);

  void draw(const DrawingScaledAttributes &attributes);
  void handle_event(const ux::Event &event);

  u16 value() const { return m_value; }
  u16 maximum() const { return m_maximum; }

  Slider &set_value(u16 value) {
    m_value = value;
    trigger_event(event_id_changed);
    update_model(var::String::number(value));
    return *this;
  }

  Slider &set_maximum(u16 value) {
    m_maximum = value;
    return *this;
  }

private:
  u16 m_value;
  u16 m_maximum;
  bool m_is_touched = false;
  void update_touch_point(const sgfx::Point display_point);

  var::String get_model_value() const {
    return Model::from_list<u16>({value(), maximum()});
  }
};

} // namespace ux

#endif // SLIDER_HPP
