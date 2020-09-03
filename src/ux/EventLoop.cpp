/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see
             // LICENSE.md for rights.
#include "ux/EventLoop.hpp"
#include "chrono.hpp"

using namespace ux;

EventLoop::EventLoop() {}

int EventLoop::loop(
  Layout &layout,
  const sgfx::Theme &theme,
  hal::Display &display) {
  m_layout = &layout;
  m_theme = &theme;
  m_display = &display;

  m_layout->set_visible_internal();
  m_update_timer.restart();
  while (1) {
    process_events();
    process_update_event();
  }

  return 0;
}

void EventLoop::process_update_event() {
  if (m_layout == nullptr) {
    return;
  }

  MicroTime elapsed = Milliseconds(m_update_timer.milliseconds());

  if (elapsed > m_update_period) {
    this->handle_event(SystemEvent(SystemEvent::id_update));
    m_update_timer.restart();
  } else {
    u32 remaining_milliseconds
      = m_update_period.milliseconds() - elapsed.milliseconds();
    // this should be an estimate until the next update event
    wait(Milliseconds(remaining_milliseconds));
  }
}

void EventLoop::handle_event(const Event &event) {
  if (m_layout) {
    m_layout->handle_event(event);
  }
}
