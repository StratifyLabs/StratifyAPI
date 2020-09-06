/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see
             // LICENSE.md for rights.
#ifndef SAPI_UX_EVENTLOOP_HPP
#define SAPI_UX_EVENTLOOP_HPP

#include "../chrono/Timer.hpp"
#include "../hal/Display.hpp"
#include "../sgfx/Theme.hpp"

namespace ux {

class Model;
class Controller;
class Event;

class EventLoop {
public:
  EventLoop();

  int loop(
    Controller &controller,
    const sgfx::Theme &theme,
    hal::Display &display);

  const chrono::Timer &timer() { return m_timer; }

  /*! \details Process events should be implemented
   * to call handle_event() for each
   * event in the system that happens.
   *
   */
  virtual void process_events() = 0;

  void trigger_event(const Event &event);

  void set_update_period(const chrono::MicroTime &duration) {
    m_update_period = duration;
  }

  const sgfx::Theme *theme() const { return m_theme; }

  hal::Display *display() { return m_display; }

  const hal::Display *display() const { return m_display; }

  const Controller *controller() const { return m_controller; }
  Controller *controller() { return m_controller; }

  const Model *model() const { return m_model; }
  Model *model() { return m_model; }

  void set_model(Model *value) { m_model = value; }

private:
  chrono::Timer m_timer;
  chrono::Timer m_update_timer;
  chrono::MicroTime m_update_period;
  Controller *m_controller = nullptr;
  Model *m_model = nullptr;
  hal::Display *m_display = nullptr;
  const sgfx::Theme *m_theme = nullptr;

  void process_update_event();
};

} // namespace ux

#endif // SAPI_UX_EVENTLOOP_HPP
