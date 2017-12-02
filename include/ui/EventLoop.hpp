/* Copyright 2014-2016 Tyler Gilbert, Inc; All Rights Reserved
 *
 */

#ifndef UI_EVENTLOOP_HPP_
#define UI_EVENTLOOP_HPP_

#include "../sys/Timer.hpp"
#include "../draw/Drawing.hpp"
#include "Element.hpp"

namespace ui {

typedef struct MCU_PACK {
	u16 hibernation_threshold;
	u16 update_period;
	u16 refresh_wait_resolution;
	u16 period;
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
	u16 hibernation_threshold() const { return m_attr.hibernation_threshold; }

	/*! \details Sets the hibernate threshold in milliseconds.
	 *
	 * @param v The hibernate threshold in milliseconds
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
	void set_hibernation_threshold(u16 v) { m_attr.hibernation_threshold = v; }

	/*! \details Accesses the period for firing the Event::UPDATE in milliseconds. */
	u16 update_period() const { return m_attr.update_period; }

	/*! \details Accesses the minimum period of the event loop in milliseconds. */
	u16 period() const { return m_attr.period; }

	/*! \details Sets the period of the event loop in milliseconds.
	 *
	 * @param v The loop period in milliseconds.
	 *
	 * The loop will delay this amount on every iteration. This determines
	 * how often events are processed (such as buttons). It is typically a much smaller value
	 * than the update period which determines how often Event::UPDATE
	 * is handled.
	 *
	 */
	void set_period(u16 v){ m_attr.period = v; }


	/*! \details Sets the Event::UPDATE period which defines
	 * how often the event loop will trigger the Event::UPDATE command for the
	 * current element.
	 *
	 * @param v The period in milliseconds
	 *
	 * The update period is less than the loop period (see set_period()), Event::UPDATE
	 * will be called on every loop.
	 *
	 */
	void set_update_period(u16 v){ m_attr.update_period = v; }

	u16 refresh_wait_resolution() const { return m_attr.refresh_wait_resolution; }
	void set_refresh_wait_resolution(u16 v){ m_attr.refresh_wait_resolution = v; }

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

	/*! \details Constructs a new event loop.
	 *
	 * @param start_element The initial element to process
	 * @param bitmap The bitmap to use for drawing
	 * @param scratch The bitmap used for animations (0 if animations are not used)
	 */
	EventLoop(ui::Element & start_element, sgfx::Bitmap & bitmap, sgfx::Bitmap * scratch = 0);


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
	 * If Element::handle_event() returns a pointer to a new element, a transtion will occur
	 * which includes:
	 * - Event::EXIT the current element
	 * - Transition (animation) to the new element
	 * - Event::ENTER the new element
	 *
	 * The loop will then continue to process_events() and fire Event::UPDATE as well as hibernate
	 * as appropriate for the new element.
	 *
	 * If Element::handle_event() returns 0 (null pointer), the event loop will stop executing. That is
	 * this method will return.
	 *
	 */
	virtual void execute();

	/*! \details This is called just before the loop starts executing.
	 * This method will send the Event::SETUP and Event::ENTER events
	 * to the primary element.
	 */
	virtual void start();

	static void start(ui::Element * element, const draw::DrawingAttr & drawing_attr);


	/*! \details This method will loop while the current element is valid.
	 * It handles sending events to the active element. See execute() for
	 * more details.
	 */
	virtual void loop();

	/*! \details Access the current element */
	Element * current_element() const { return m_current_element; }

	/*! \details Set the current element */
	void set_current_element(Element * v){ m_current_element = v; }

	/*! \details Access the drawing attributes */
	draw::DrawingAttr & drawing_attr(){ return m_drawing_attr; }

	/*! \details Update the drawing attributes */
	void set_drawing_attr(const draw::DrawingAttr & attr){ m_drawing_attr = attr; }

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

	static Element * handle_event(Element * current_element, const Event & event, const draw::DrawingAttr & drawing_attr);

	static void handle_transition(Element * current_element, Element * next_element, const draw::DrawingAttr & drawing_attr);

protected:
	bool handle_event(const ui::Event & event);

private:

	draw::DrawingAttr m_drawing_attr;
	Element * m_current_element;
	sys::Timer m_update_timer;
	sys::Timer m_loop_timer;

	u16 m_loop_delay;



};

} /* namespace ui */

#endif /* UI_EVENTLOOP_HPP_ */
