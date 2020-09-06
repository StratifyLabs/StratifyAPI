/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see
// LICENSE.md for rights.
#ifndef SAPI_UX_TOUCHGESTURE_HPP
#define SAPI_UX_TOUCHGESTURE_HPP

#include "../chrono/Timer.hpp"
#include "../sgfx/Point.hpp"
#include "../sgfx/Region.hpp"
#include "Event.hpp"

namespace ux {

class TouchContext {
public:
  enum event_ids {
    event_id_none,
    event_id_active,
    event_id_pressed,
    event_id_released,
    event_id_dragged,
    event_id_touched,
    event_id_completed
  };

  EVENT_DECLARE_TYPE()

private:
  API_AC(TouchContext, sgfx::Point, point);
};

/*! \brief TouchGesture Class
 * \details The TouchGesture class
 * processes touch events to determine
 * if the touch was a
 *
 * - Touch (press and release)
 * - Hold (press and hold for a while)
 * - Drag (press and drag either vertically or horizontally)
 *
 */
class TouchGesture {
public:
  TouchGesture &set_vertical_drag_enabled(bool value = true) {
    m_is_vertical_drag_enabled = value;
    return *this;
  }

  TouchGesture &set_horizontal_drag_enabled(bool value = true) {
    m_is_horizontal_drag_enabled = value;
    return *this;
  }

  TouchGesture &set_multidimensional_drag_enabled(bool value = true) {
    m_is_multidimensional_drag_enabled = value;
    return *this;
  }

  TouchGesture &set_region(const sgfx::Region &region) {
    m_region = region;
    return *this;
  }

  enum TouchContext::event_ids process(const Event &event);

  const chrono::Timer &timer() const { return m_timer; }

  const sgfx::Point &drag() const { return m_drag; }

  bool is_vertical_drag_enabled() const { return m_is_vertical_drag_enabled; }

  bool is_horizontal_drag_enabled() const {
    return m_is_horizontal_drag_enabled;
  }

  bool is_multidimensional_drag_enabled() const {
    return m_is_multidimensional_drag_enabled;
  }

private:
  sgfx::Point m_drag;
  sgfx::Point m_last_point;
  sgfx::Region m_region;
  chrono::Timer m_timer;
  s16 m_vertical_drag = 0;
  s16 m_horizontal_drag = 0;
  bool m_is_vertical_drag_enabled = false;
  bool m_is_horizontal_drag_enabled = false;
  bool m_is_multidimensional_drag_enabled = false;
  bool m_is_vertical_drag_active = false;
  bool m_is_horizontal_drag_active = false;
  bool m_is_pressed_contained = false;
  static const sg_int_t m_drag_theshold = 1;

  sgfx::Point process_drag(const sgfx::Point &point);
};

} // namespace ux

#endif // SAPI_UX_TOUCHGESTURE_HPP
