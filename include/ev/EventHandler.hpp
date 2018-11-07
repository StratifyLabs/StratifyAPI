/* Copyright 2014-2018 Tyler Gilbert, Inc; All Rights Reserved
 *
 */

#ifndef EV_EVENTHANDLER_HPP
#define EV_EVENTHANDLER_HPP

#include "../api/EvObject.hpp"
#include "Event.hpp"

namespace ui {
class EventLoop;
}

namespace ev {

class EventLoop;

/*! \brief Event Handler Class
 *
 */
class EventHandler : public api::EvWorkObject {
public:
	EventHandler();

	/*! \details Handles an event for the currently active EventHandler.
	  *
	  * @param event The event to execute
	  * @return Determines if the ev::EventLoop transitions to another EventHandler
	  * or stays on this one.
	  *
	  *
	  * ev::EventLoop::execute() and ev::EventLoop::run() execute this method
	  * each time an event happens (e.g., ev::Event::UPDATE, ev::Event::SETUP, ev::Event::BUTTON_PRESSED).
	  *
	  * If this method returns a pointer to another EventHandler, then that event handler
	  * becomes the active ev::EventHandler.
	  *
	  */
	virtual EventHandler * handle_event(const Event & event);


	/*! \details Returns a pointer to the element's event loop.
	  *
	  * If the element isn't running in an event loop, this
	  * method will return 0.
	  *
	  */
	EventLoop * event_loop() const { return m_event_loop; }

protected:

	friend class EventLoop;
	friend class ui::EventLoop;
	void set_event_loop(EventLoop * event_loop){ m_event_loop = event_loop; }

private:
	EventLoop * m_event_loop;

};

}

#endif // EV_EVENTHANDLER_HPP
