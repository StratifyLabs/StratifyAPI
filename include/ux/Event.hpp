/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see
             // LICENSE.md for rights.
/* Copyright 2014-2018 Tyler Gilbert, Inc; All Rights Reserved
 *
 */
#ifndef SAPI_UX_EVENT_HPP_
#define SAPI_UX_EVENT_HPP_

#include <mcu/types.h>

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
