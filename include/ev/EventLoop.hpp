/* Copyright 2014-2018 Tyler Gilbert, Inc; All Rights Reserved
 *
 */

#ifndef SM_EVENTLOOP_HPP_
#define SM_EVENTLOOP_HPP_

#include "../sys/Timer.hpp"
#include "EventHandler.hpp"
#include "Event.hpp"

namespace ev {

typedef struct MCU_PACK {
    u16 hibernation_threshold_msec;
    u16 update_period_msec;
    u16 refresh_wait_resolution_usec;
    u16 period_msec;
} event_loop_attr_t;


/*! \brief Event Loop Attributes
 * \details This class defines attributes that apply to an ui::EventLoop.
 */
class EventLoopAttr {
public:

	/*! \details Construct new Event Loop attributes */
	EventLoopAttr();

	operator event_loop_attr_t() const { return m_attr; }

	/*! \details Accesses the hibernate timeout in milliseconds. */
    u16 hibernation_threshold() const { return m_attr.hibernation_threshold_msec; }

	/*! \details Sets the hibernate threshold in milliseconds.
	 *
	 * @param msec The hibernate threshold in milliseconds
	 *
	 * If the update period (set_update_period()) is greater than
	 * the hibernation threshold, the event loop will execute the kernel
	 * request for hibernation. If the kernel request is not handled,
	 * the event loop will put the system in hibernate.  The logic looks like this:
	 *
	 * \code
	 * if( update_period() > hibernation_threshold() ){
	 * 	if( Sys::request(SAPI_REQUEST_HIBERNATE) < 0 ){
	 * 		Sys::hibernate( update_period() / 1000 );  //kernel hibernate will wake on an RTC alarm if the RTC is present
	 * 	}
	 * }
	 * \endcode
	 *
	 * This logic allows the BSP (which implements the kernel_request() function called by Sys::request()) to override
	 * or delay the hibernation until other applications or tasks are ready.
	 *
	 * By default, the hibernation threshold is set to the max value (65536) effectively
	 * disabling hibernation.
	 *
	 * The system will stay in hibernation mode until a hardware interrupt
	 * wakes the processor. This interrupt typically needs to be a external
	 * pin interrupt or RTC alarm because most peripherals are off during hiberation.
	 *
	 *
	 */
    void set_hibernation_threshold(const sys::MicroTime & value){ m_attr.hibernation_threshold_msec = value.msec(); };
    void set_hibernation_threshold(u16 msec) { m_attr.hibernation_threshold_msec = msec; }

	/*! \details Accesses the period for firing the Event::UPDATE in milliseconds. */
    u16 update_period() const { return m_attr.update_period_msec; }

	/*! \details Accesses the minimum period of the event loop in milliseconds. */
    u16 period() const { return m_attr.period_msec; }

	/*! \details Sets the period of the event loop in milliseconds.
	 *
	 * @param msec The loop period in milliseconds.
	 *
	 * The loop will delay this amount on every iteration. This determines
	 * how often events are processed (such as buttons). It is typically a much smaller value
	 * than the update period which determines how often Event::UPDATE
	 * is handled.
	 *
	 */
    void set_period(u16 msec){ m_attr.period_msec = msec; }


	/*! \details Sets the Event::UPDATE period which defines
	 * how often the event loop will trigger the Event::UPDATE command for the
	 * current element.
	 *
	 * @param msec The period in milliseconds
	 *
	 * If the update period is less than the loop period (see set_period()), Event::UPDATE
	 * will be called on every loop at the loop period interval.
	 *
	 */
    void set_update_period(u16 msec){ m_attr.update_period_msec = msec; }

	/*! \details Accesses the display refresh wait resolution time in microseconds.
	 *
	 * The loop will always wait for the display driver to complete its refresh before
	 * handling events that modify the display memory. This value determines how long to sleep
	 * between polling events to the display driver.
	 *
	 */
    u16 refresh_wait_resolution() const { return m_attr.refresh_wait_resolution_usec; }

	/*! \details Sets the value of the display refresh wait resolution in microseconds.
	 *
	 * @param usec The number of microseconds to between display driver polling events
	 *
	 * See refresh_wait_resolution() for more details.
	 */
    void set_refresh_wait_resolution(u16 usec){ m_attr.refresh_wait_resolution_usec = usec; }

protected:
	event_loop_attr_t m_attr;
};

/*! \brief Event Loop Class
 * \details This class executes an event loop.  The events are passed to the
 * current_element() and handled using Element::handle_event().  If Element::handle_event() returns a
 * pointer to a new element, the event loop will Event::EXIT the current element
 * and Event::ENTER the new element and execute an animation to the new event.
 *
 * The EventLoop is an abstract class, and you must implement
 * the process_events() method which needs to process user events
 * that are specific to the system.
 *
 * \code
 * ui::ButtonPin select_button(0,1); //select button is on pin 0.1
 *
 * MyEventLoop::MyEventLoop(ui::Element & start_element, sgfx::Bitmap & bitmap, sgfx::Bitmap * scratch) :
 * 	EventLoop(start_element, bitmap, scratch){
 * }
 *
 * void MyEventLoop::process_events() {
 * 	select_button.update();
 * 	handle_event(select_button.event());
 * }
 * \endcode
 *
 *
 */
class EventLoop: public EventLoopAttr {
public:

    /*! \details Constructs a new headless (no display) event loop.
     *
     * @param start_element The initial element to process
     *
     * The EventLoop will typically update the display when the current_element()
     * indicates that it should. By initializing an EventLoop using this constructor
     * the display is ignored.
     *
     */
    EventLoop(EventHandler & start_event_handler);

	/*! \details Executes the event loop.
	 *
	 * First the event loop will have the current element handle Event::SETUP.
	 *
	 * The loop will then transition (execute the animation) for the first
	 * element then Event::ENTER.
	 *
	 * The loop will then loop firing Event::UPDATE every update_period() milliseconds.
	 * If hibernation_threshold() is less than update_period(), the loop will attempt
	 * to put the processor in hibernation periodically to save power.
	 *
	 * If update_period() is less then hibernation_threshold(), the loop will execute
	 * process_events() every period() milliseconds.
	 *
     * If EventHandler::handle_event() returns a pointer to a new element, a transtion will occur
	 * which includes:
	 * - Event::EXIT the current element
	 * - Transition (animation) to the new element
	 * - Event::ENTER the new element
	 *
	 * The loop will then continue to process_events() and fire Event::UPDATE as well as hibernate
	 * as appropriate for the new element.
	 *
     * If EventHandler::handle_event() returns 0 (null pointer), the event loop will stop executing. That is
	 * this method will return.
	 *
	 */
	virtual void execute();

	/*! \details This is called just before the loop starts executing.
	 * This method will send the Event::SETUP and Event::ENTER events
	 * to the primary element.
	 */
	virtual void start();

    static void start(EventHandler * event_handler);


	/*! \details This method will loop while the current element is valid.
	 * It handles sending events to the active element. See execute() for
	 * more details.
	 */
	virtual void loop();

    /*! \details Accesses the current event handler. */
    EventHandler * current_event_handler() const { return m_current_event_handler; }

    /*! \details Sets the current event handler. */
    void set_current_event_handler(EventHandler * v){ m_current_event_handler = v; }

	/*! \details This method should call handle_event for
	 * each possible event in the event loop.
	 *
	 * \code
	 *
	 * Button * up_button;
	 * Button * down_button;
	 *
	 * void MyEventLoop::process_events(){
	 *
	 * 	handle_event(up_button->event());
	 * 	handle_event(down_button->event());
	 * 	//call handle event on any other events -- Event loop handles SETUP, ENTER, and UPDATE
	 *
	 * }
	 *
	 * \endcode
	 *
	 */
	virtual void process_events() = 0;

    /*! \details Is executed when the event handler returns a null event and
     * the current event is set to null.
     *
     * @param last_event_handler The event handler that returned a null handler.
     *
     * The default behavior is to exit from the EventLoop (execute() method). This
     * method can be reimplemented by the application to update the event handler
     * based on the value and state of last_event_handler.
     *
     */
    virtual EventHandler * catch_null_handler(EventHandler * last_event_handler){ return 0; }

    //maybe these should be private and accessed through Friends?
    static EventHandler * handle_event(EventHandler * current_event_handler, const Event & event, EventLoop * event_loop = 0);
    static void handle_transition(EventHandler * current_event_handler, EventHandler * next_event_handler);

protected:

    /*! \details Handles the specified event.
     *
     * @param event Event to handle within the loop
     *
     * This method should be called by process_events() whenever
     * the system triggers an event.  For example, if
     * the system needs to just handle an up button
     * or down button press:
     *
     * \code
     *
     * #include <sapi/hal.hpp>
     *
     * ButtonPin up_button(0,0);
     * ButtonPin down_button(0,1)
     *
     * void MyEventLoop::process_events(){
     *  up_button.update(); //refresh button status
     *  down_button.update();
     *
     *  handle_event(up_button.event());
     *  handle_event(down_button.event());
     *
     * }
     * \endcode
     *
     */
    virtual bool handle_event(const Event & event);

    void check_loop_for_hibernate();
    void check_loop_for_update();

    sys::Timer & update_timer(){ return m_update_timer; }
    sys::Timer & loop_timer(){ return m_loop_timer; }

private:
    EventHandler * m_current_event_handler;
	sys::Timer m_update_timer;
	sys::Timer m_loop_timer;

};

} /* namespace ui */

#endif /* SM_EVENTLOOP_HPP_ */
