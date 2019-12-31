/* Copyright 2014-2018 Tyler Gilbert, Inc; All Rights Reserved
 *
 */
#ifndef SAPI_UX_EVENT_HPP_
#define SAPI_UX_EVENT_HPP_

#include <sos/dev/button.h>
#include "../var/Data.hpp"
#include "../var/Array.hpp"

namespace sys {
class Signal;
}

namespace ux {


class EventType {
public:
   EventType(){}

   EventType(const char type[4]){
      m_type = (type[0] << 0);
      m_type |= (type[1] << 8);
      m_type |= (type[2] << 16);
      m_type |= (type[3] << 24);
   }

   u32 type() const {
      return m_type;
   }


private:
   u32 m_type;
};

/*! \brief Event Class
 * \details This class defines action-able events (such as
 * button presses) that occur within ui::EventLoop and are handled
 * by ui::Element::handle_event().
 *
 * The event includes the type and a pointer to an object associated with the event. For
 * example, events of type Event::SIGNAL will include a pointer to a sys::Signal
 * object.
 *
 *
 */
class Event {
public:

	Event();
	Event(const EventType & type, u32 id);
	Event(const EventType & type, u32 id, void * context);

	Event & set_type(const EventType & type){
		m_type = type;
		return *this;
	}

	const EventType & type() const { return m_type; }

	u32 id() const {
		return m_id;
	}


private:
	EventType m_type;
	u32 m_id;
	void * m_context;
};


class SystemEvent : public Event {
public:
	SystemEvent(u32 id) : Event("_sys", id){}

	enum id {
		id_enter,
		id_exit,
		id_update
	};

};

class TouchEvent : public Event {
public:
	TouchEvent(u32 id) : Event("touc", id){}

	enum id {
		id_none,
		id_pressed,
		id_released,
	};



private:


};


}

#endif /* SAPI_UX_EVENT_HPP_ */
