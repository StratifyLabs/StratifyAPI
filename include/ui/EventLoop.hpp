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
	u16 hibernate_timeout;
	u16 update_period;
	u16 refresh_wait_resolution;
	u16 period;
} event_loop_attr_t;


/*! \brief Event Loop Attributes
 * \details This class defines attributes that apply to an ui::EventLoop.
 */
class EventLoopAttr {
public:
	EventLoopAttr();

	operator event_loop_attr_t() const { return m_attr; }

	/*! \details This method returns true if hibernate is enabled (hibernate timeout is non-zero) */
	bool is_hibernate() const { return (m_attr.hibernate_timeout != 0); }

	/*! \details This method returns the hibernate timeout in milliseconds */
	u16 hibernate_timeout() const { return m_attr.hibernate_timeout; }

	/*! \details This method sets the hibernate timeout in milliseconds */
	void set_hibernate_timeout(u16 v) { m_attr.hibernate_timeout = v; }

	/*! \details This method returns the period for firing the Event::UPDATE */
	u16 update_period() const { return m_attr.update_period; }

	/*! \details This method returns the period for the loop */
	u16 period() const { return m_attr.period; }

	/*! \details This method sets the Event loop period.
	 *
	 * @param v The loop period in milliseconds.
	 */
	void set_period(u16 v){ m_attr.period = v; }


	/*! \details This method set the Event::UPDATE period which defines
	 * how often the event loop will trigger the Event::UPDATE command for the
	 * current element.
	 *
	 * @param v The period in milliseconds
	 *
	 */
	void set_update_period(u16 v){ m_attr.update_period = v; }

	u16 refresh_wait_resolution() const { return m_attr.refresh_wait_resolution; }
	void set_refresh_wait_resolution(u16 v){ m_attr.refresh_wait_resolution = v; }

protected:
	event_loop_attr_t m_attr;
};

/*! \brief Event Loop Class
 * \details This class processes events.
 */
class EventLoop: public EventLoopAttr {
public:

	/*! \details Construct a new Event loop */
	EventLoop(ui::Element & start_element, sgfx::Bitmap & bitmap, sgfx::Bitmap * scratch = 0);


	/*! \details Execute the event loop */
	virtual void execute();

	/*! \details This is called just before the loop starts executing.
	 * This method will send the Event::SETUP and Event::ENTER events
	 * to the primary element.
	 */
	virtual void start();

	/*! \details This method will loop while the current element is valid.
	 * It handles sending events to the active element.  Event::UPDATE
	 * is sent based on upate_period().  Whenever the active element
	 * changes, Event::ENTER is sent to the new element.
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
	 * void MyEventLoop::handle_events(){
	 *
	 * 	handle_event(up_button->event());
	 * 	handle_event(down_button->event());
	 * 	//call handle event on any other events -- Event loop handles SETUP, ENTER, and UPDATE
	 *
	 * }
	 *
	 */
	virtual void process_events() = 0;

	virtual void handle_element_changed(){}

	static Element * handle_event(Element * current_element, const Event & event, const draw::DrawingAttr & drawing_attr);


protected:
	void handle_event(const ui::Event & event);

private:

	draw::DrawingAttr m_drawing_attr;
	Element * m_current_element;
	//sys::Timer m_loop_timer;
	sys::Timer m_update_timer;



};

} /* namespace ui */

#endif /* UI_EVENTLOOP_HPP_ */
