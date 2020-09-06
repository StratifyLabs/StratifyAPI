/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see
             // LICENSE.md for rights.
/* Copyright 2014-2018 Tyler Gilbert, Inc; All Rights Reserved
 *
 */
#ifndef SAPI_UX_EVENT_HPP_
#define SAPI_UX_EVENT_HPP_

#include "../api/ApiObject.hpp"

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

  bool operator==(const Event &event) const {
    return (event.type() == type() && event.id() == id());
  }

  bool operator!=(const Event &event) const { return !(*this == event); }

  Component *component() const {
    return reinterpret_cast<Component *>(m_context);
  }

  template <class T> T *is_trigger() const {
    if (type() == T::event_type()) {
      return reinterpret_cast<T *>(context());
    }
    return nullptr;
  }
  template <class T> T *is_trigger(u32 id) const {
    if ((type() == T::event_type()) && (this->id() == id)) {
      return reinterpret_cast<T *>(context());
    }
    return nullptr;
  }

  template <class T> static Event get_event(u32 id, T *context) {
    return Event().set_type(T::event_type()).set_id(id).set_context(context);
  }

private:
  API_AF(Event, u32, type, 0);
  API_AF(Event, u32, id, 0);
  API_AF(Event, void *, context, nullptr);
};

#define EVENT_DECLARE_TYPE()                                                   \
  static u32 event_type() { return reinterpret_cast<u32>(event_type); }

class SystemEvent : public Event {
public:
  EVENT_DECLARE_TYPE()

  SystemEvent() {
    set_type(event_type());
    set_id(event_id_none);
  }
  SystemEvent(u32 id) {
    set_type(event_type());
    set_id(id);
  }
  enum event_ids {
    event_id_none,
    event_id_enter,
    event_id_exit,
    event_id_periodic
  };
};

} // namespace ux

#endif /* SAPI_UX_EVENT_HPP_ */
