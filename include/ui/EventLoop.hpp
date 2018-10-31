/* Copyright 2014-2016 Tyler Gilbert, Inc; All Rights Reserved
 *
 */

#ifndef UI_EVENTLOOP_HPP_
#define UI_EVENTLOOP_HPP_

#include "../sys/Timer.hpp"
#include "../draw/Drawing.hpp"
#include "Element.hpp"
#include "../ev/EventLoop.hpp"

namespace ui {

typedef ev::event_loop_attr_t event_loop_attr_t;
typedef ev::EventLoopAttributes EventLoopAttributes;


/*! \brief Event Loop Class (Graphical UI)
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
class EventLoop: public ev::EventLoop {
public:

	/*! \details Constructs a new event loop.
	 *
	 * @param start_element The initial element to process
	 * @param bitmap The bitmap to use for drawing
	 * @param scratch The bitmap used for animations (0 if animations are not used)
	 */
	EventLoop(ui::Element & start_element, sgfx::Bitmap & bitmap, sgfx::Bitmap * scratch = 0);

	//re-implement start to update the display
	virtual void start();

	static void start(ui::Element * element, const draw::DrawingAttr & drawing_attr);

	//re-implement loop to update the display
	virtual void loop();

	/*! \details Accesses the current element. */
	Element * current_element() const { return (Element*)ev::EventLoop::current_event_handler(); }

	/*! \details Sets the current element. */
	void set_current_element(Element * v){ ev::EventLoop::set_current_event_handler(v); }

	/*! \details Accesses the drawing attributes. */
	draw::DrawingAttr & drawing_attr(){ return m_drawing_attr; }

	/*! \details Updates the drawing attributes. */
	void set_drawing_attributes(const draw::DrawingAttr & attributes){ m_drawing_attr = attributes; }
	void set_drawing_attr(const draw::DrawingAttr & attributes){ set_drawing_attributes(attributes); }

	virtual Element * catch_null_handler(Element * last_element){ return 0; }


	static Element * handle_event(Element * current_element, const Event & event, const draw::DrawingAttr & drawing_attr, EventLoop * event_loop = 0);
	static void handle_transition(Element * current_element, Element * next_element, const draw::DrawingAttr & drawing_attr);

protected:

	bool handle_event(const ui::Event & event);

private:
	draw::DrawingAttr m_drawing_attr;

};

} /* namespace ui */

#endif /* UI_EVENTLOOP_HPP_ */
