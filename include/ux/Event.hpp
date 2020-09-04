/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see
             // LICENSE.md for rights.
/* Copyright 2014-2018 Tyler Gilbert, Inc; All Rights Reserved
 *
 */
#ifndef SAPI_UX_EVENT_HPP_
#define SAPI_UX_EVENT_HPP_

#include "../var/Array.hpp"
#include "../var/Data.hpp"
#include <sos/dev/button.h>

namespace sys {
class Signal;
}

namespace ux {

class Component;

#define EVENT_TYPE(a, b, c, d) ((a << 24) | (b << 16) | (c << 8) | (d))

/*! \brief Event Class
 * \details This class defines action-able events (such as
 * button presses) that occur within ui::EventLoop and are handled
 * by ui::Element::handle_event().
 *
 * The event includes the type and a pointer to an object associated with the
 * event. For example, events of type Event::SIGNAL will include a pointer to a
 * sys::Signal object.
 *
 *
 */
class Event {
public:
  Event();
  Event(u32 type, u32 id, Component *component = nullptr);

  Event &set_type(const char type[4]) {
    m_type = (type[0] << 0);
    m_type |= (type[1] << 8);
    m_type |= (type[2] << 16);
    m_type |= (type[3] << 24);
    return *this;
  }

  Event &set_type(u32 type) {
    m_type = type;
    return *this;
  }

  u32 type() const { return m_type; }

  u32 id() const { return m_id; }

  bool operator==(const Event &event) const {
    return (event.type() == type() && event.id() == id());
  }

  bool operator!=(const Event &event) const { return !(*this == event); }

  template <typename T> const T &reinterpret() const {
    return static_cast<const T &>(*this);
  }

  Component *component() const { return m_component; }

private:
  u32 m_type;
  u32 m_id;
  Component *m_component;
};

template <class D, u32 T> class EventObject : public Event {
public:
  EventObject(u32 id, D *component) : Event(T, id, component) {}

  static u32 event_type() { return T; }

  static bool is_event(const Event &event, u32 id) {
    return (event.type() == event_type()) && (event.id() == id);
  }

  static D *match_component(const Event &event, u32 id) {
    if (is_event(event, id)) {
      return static_cast<D *>(event.component());
    }
    return nullptr;
  }

  static D *match_component(const Event &event) {
    if (event.type() == event_type()) {
      return static_cast<D *>(event.component());
    }
    return nullptr;
  }
};

class SystemEvent
  : public EventObject<Component, EVENT_TYPE('_', 's', 'y', 's')> {
public:
  SystemEvent() : EventObject(id_none, nullptr) {}
  SystemEvent(u32 id) : EventObject(id, nullptr) {}
  enum system_id { id_none, id_enter, id_exit, id_update };

  static enum system_id get_system_id(const Event &event) {
    if (event.type() == SystemEvent::event_type()) {
      return static_cast<enum system_id>(event.id());
    }
    return id_none;
  }
};

} // namespace ux

#endif /* SAPI_UX_EVENT_HPP_ */
