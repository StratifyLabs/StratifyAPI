/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see
             // LICENSE.md for rights.
/* Copyright 2014-2018 Tyler Gilbert, Inc; All Rights Reserved
 *
 */
#ifndef SAPI_UX_EVENT_HPP_
#define SAPI_UX_EVENT_HPP_

#include "../var/Array.hpp"
#include "../var/Data.hpp"

namespace sys {
class Signal;
}

namespace ux {

class Component;

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
  Event(u32 type, u32 id, void *context = nullptr);

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

  Component *component() const {
    return reinterpret_cast<Component *>(m_context);
  }

  void *context() const { return m_context; }

private:
  u32 m_type;
  u32 m_id;
  void *m_context;
};

template <class D, const char literal_id[5]> class EventAccess : public Event {
public:
  EventAccess(u32 id, D *component) : Event(event_type(), id, component) {
    static_assert(sizeof(EventAccess<D, literal_id>) == sizeof(Event));
  }

  static u32 event_type() {
    return literal_id[0] | literal_id[1] << 8 | literal_id[2] << 16
           | literal_id[3] << 24;
  }

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

class SystemEvent : public Event {
public:
  static u32 event_type() { return reinterpret_cast<u32>(event_type); }

  SystemEvent() : Event(event_type(), event_id_none, nullptr) {}
  SystemEvent(u32 id) : Event(event_type(), id, nullptr) {}
  enum event_ids {
    event_id_none,
    event_id_entered,
    event_id_exiteded,
    event_id_updated
  };
};

} // namespace ux

#endif /* SAPI_UX_EVENT_HPP_ */
