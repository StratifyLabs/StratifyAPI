/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see
             // LICENSE.md for rights.
#include "ux/EventLoop.hpp"
#include "chrono.hpp"
#include "ux/Controller.hpp"
#include "ux/Model.hpp"

using namespace ux;

EventLoop::EventLoop(
  Controller &controller,
  Model &model,
  hal::Display &display,
  const sgfx::Theme &theme)
  : m_controller(controller), m_model(model), m_display(&display),
    m_theme(&theme) {}

int EventLoop::loop() {
  m_controller.distribute_event(SystemEvent(SystemEvent::event_id_enter));
  m_update_timer.restart();
  while (1) {
    process_events();
    process_update_event();
  }

  return 0;
}

void EventLoop::process_update_event() {
  MicroTime elapsed = Milliseconds(m_update_timer.milliseconds());

  if (elapsed > m_update_period) {
    m_controller.distribute_event(SystemEvent(SystemEvent::event_id_periodic));
    m_update_timer.restart();
  } else {
    u32 remaining_milliseconds
      = m_update_period.milliseconds() - elapsed.milliseconds();
    // this should be an estimate until the next update event
    wait(Milliseconds(remaining_milliseconds));
  }

  std::swap(m_temporary_event_stack, m_event_stack);

  while (m_temporary_event_stack.count() > 0) {
    m_controller.distribute_event(m_temporary_event_stack.top());
    m_temporary_event_stack.pop();
  }
}

void EventLoop::trigger_event(const Event &event) {
  API_ASSERT(event.type() != SystemEvent::event_type());
  m_event_stack.push(event);
}
