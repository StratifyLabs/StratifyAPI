/* Copyright 2014-2018 Tyler Gilbert, Inc; All Rights Reserved
 *
 */

#ifndef SM_EVENTHANDLER_HPP
#define SM_EVENTHANDLER_HPP

#include "Event.hpp"

namespace ui {
class EventLoop;
}

namespace  sm {

class EventLoop;

class EventHandler
{
public:
    EventHandler();

    /*! \details This method is called on the current Element when an event (e.g., Element::ENTER,
     * Element::UPDATE, Element::TOP_PRESS) happens.
     *
     * @param event The event to execute
     * @param attr The drawing attributes to use for the event
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

#endif // SM_EVENTHANDLER_HPP
