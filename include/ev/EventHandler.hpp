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

    /*! \details This method is called on the current Element when an event (e.g., Element::ENTER,
     * Element::UPDATE, Element::TOP_PRESS) happens.
     *
     * @param event The event to execute
     * @return For transitions, the new element is returned; otherwise this
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
