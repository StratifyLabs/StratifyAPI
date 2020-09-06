/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see
             // LICENSE.md for rights.
#include "ux/EventLoop.hpp"
#include "chrono.hpp"
#include "ux/Controller.hpp"

using namespace ux;

EventLoop::EventLoop() {}

int EventLoop::loop(
  Controller &controller,
  const sgfx::Theme &theme,
  hal::Display &display) {
  m_controller = &controller;
  m_theme = &theme;
  m_display = &display;

  m_controller->distribute_event(SystemEvent(SystemEvent::event_id_entered));
  m_update_timer.restart();
  while (1) {
    process_events();
    process_update_event();
  }

  return 0;
}

void EventLoop::process_update_event() {
  if (m_controller == nullptr) {
    return;
  }

  MicroTime elapsed = Milliseconds(m_update_timer.milliseconds());

  if (elapsed > m_update_period) {
    m_controller->distribute_event(SystemEvent(SystemEvent::event_id_updated));
    m_update_timer.restart();
  } else {
    u32 remaining_milliseconds
      = m_update_period.milliseconds() - elapsed.milliseconds();
    // this should be an estimate until the next update event
    wait(Milliseconds(remaining_milliseconds));
  }
}

void EventLoop::trigger_event(const Event &event) {
  API_ASSERT(event.type() != SystemEvent::event_type());
  m_controller->distribute_event(event);
}
