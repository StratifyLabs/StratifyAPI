/* Copyright 2014-2018 Tyler Gilbert, Inc; All Rights Reserved
 *
 */
#ifndef SAPI_EV_EVENT_HPP_
#define SAPI_EV_EVENT_HPP_

#include <sos/dev/button.h>
#include "../api/EvObject.hpp"

namespace ui {
class ListItem;
class Element;
}

namespace sys {
class Signal;
}

namespace ev {
class Button;

class EventHandler;

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
class Event : public api::EvInfoObject {
public:

	enum {
		FLAG_IS_BUTTON = 0x8000,
		BUTTON_FLAG = FLAG_IS_BUTTON,
		FLAG_IS_LIST_ITEM = 0x4000,
		LIST_ITEM_FLAG = FLAG_IS_LIST_ITEM,
		FLAG_IS_EVENT_HANDLER = 0x1000,
		FLAG_IS_ELEMENT = 0x2000 | FLAG_IS_EVENT_HANDLER, //element inherits EventHandler so it is either one

	};

	/*! \details The event type */
	enum event_type {
		NONE = 0,
		SETUP /*! This event is called at startup after all objects have been constructed */ = 1,
		ENTER /*! This event is called when the element becomes active */ = 2,
		UPDATE /*! This event is called in a loop while the element is active */ = 3,
		BUTTON_ACTUATED /*! This event is called when a button is actuated (pressed and released). Use button() to access button details. */ = FLAG_IS_BUTTON | 4,
		BUTTON_ACTUATION = BUTTON_ACTUATED,
		BUTTON_HELD /*! This event is called when a button is held. Use button() to access button details. */ = FLAG_IS_BUTTON | 5,
		BUTTON_HOLD = BUTTON_HELD,
		BUTTON_PRESSED /*! This event is called when a button is pressed. Use button() to access button details. */ = FLAG_IS_BUTTON | 6,
		BUTTON_RELEASED /*! This event is called when a button is released. Use button() to access button details. */ = FLAG_IS_BUTTON | 7,
		NETWORK_DATA /*! This event is called when data arrives on the network */ = 8,
		SIGNAL_RECEIVED /*! This event is called when the process receives a signal */ = 9,
		SIGNAL = SIGNAL_RECEIVED,
		APPLICATION /*! This event is application specific where the data is specified by the application */ = 10,
		LIST_ITEM_SELECTED /*! Select an item in a list */ = FLAG_IS_LIST_ITEM | 11,
		LIST_ITEM_ACTUATED /*! Actuate an item in a list */ = FLAG_IS_LIST_ITEM | 12,
		LIST_ACTUATED /*! Select an item in a list or menu */ = 13,
		LIST_ACTUATE = 13,
		LIST_UP /*! Scroll up in a list or menu */ = 14,
		LIST_DOWN /*! Scroll down in a list or menu */ = 15,
		SCROLL_UP /*! Scroll up (same as LIST_UP) */ = LIST_UP,
		SCROLL_DOWN /*! Scroll down (same as LIST_DOWN) */ = LIST_DOWN,
		MENU_BACK /*! Go back in the menu */ = FLAG_IS_ELEMENT | 16,
		TAB_LEFT /*! Slide to the tab on the left */ = FLAG_IS_ELEMENT | 17,
		TAB_RIGHT /*! Slide to the tab on the left */ = FLAG_IS_ELEMENT | 18,
		MENU_ACTUATED /*! Select the item in the menu */ = FLAG_IS_ELEMENT | 19,
		MENU_ACTUATE = 19,
		EXIT /*! The application transitioned to a new element (last event on current element) */ = FLAG_IS_ELEMENT | 20


		//EVENT_TYPE_TOTAL //omit TOTAL so that the compiler doesn't complain about not handling the case

	};

	/*! \details Button definitions */
	enum button_id {
		NO_BUTTON /*! No Button */ = BUTTON_NONE,
		UP_BUTTON /*! Up Button */ = BUTTON_UP,
		DOWN_BUTTON /*! Down Button */ = BUTTON_DOWN,
		LEFT_BUTTON /*! LeftButton */ = BUTTON_LEFT,
		RIGHT_BUTTON /*! Right Button */ = BUTTON_RIGHT,
		SELECT_BUTTON /*! Select Button */ = BUTTON_SELECT,
		BACK_BUTTON /*! Back Button */ = BUTTON_BACK,
		EXIT_BUTTON /*! Exit Button */ = BUTTON_EXIT,
		USER_BUTTON0 /*! User button 0 */ = BUTTON_USER0,
		USER_BUTTON1 /*! User button 1 */ = BUTTON_USER1,
		USER_BUTTON2 /*! User button 2 */ = BUTTON_USER2,
		USER_BUTTON3 /*! User button 3 */ = BUTTON_USER3,
		USER_BUTTON4 /*! User button 4 */ = BUTTON_USER4,
		USER_BUTTON5 /*! User button 5 */ = BUTTON_USER5,
		USER_BUTTON6 /*! User button 6 */ = BUTTON_USER6,
		USER_BUTTON7 /*! User button 7 */ = BUTTON_USER7,
		USER_BUTTON8 /*! User button 8 */ = BUTTON_USER8,
		USER_BUTTON9 /*! User button 9 */ = BUTTON_USER9,
		USER_BUTTON10 /*! User button 10 */ = BUTTON_USER10,
		USER_BUTTON11 /*! User button 11 */ = BUTTON_USER11,
		USER_BUTTON12 /*! User button 12 */ = BUTTON_USER12,
		USER_BUTTON13 /*! User button 13 */ = BUTTON_USER13,
		USER_BUTTON14 /*! User button 14 */ = BUTTON_USER14,
		USER_BUTTON15 /*! User button 15 */ = BUTTON_USER15,

		//EVENT_BUTTON_TOTAL //omit TOTAL so that the compiler doesn't complain about not handling the case
	};


	Event();

	/*! \details Construct a new event with the specified type and object */
	Event(
			enum event_type type,
			void * object = 0
			){
		m_type = type;
		m_objects.object = object;
	}

	/*! \details Constructs a new event referencing a button.
	 *
	 * @param type The type of event (should be one of BUTTON_* events)
	 * @param button A pointer to the button which caused the event.
	 */
	Event(enum event_type type, ev::Button * button){
		m_type = type;
		m_objects.button = button;
	}

	Event(enum event_type type, ui::Element * element){
		m_type = type;
		m_objects.element = element;
	}

	EventHandler * event_handler() const {
		if( m_type & FLAG_IS_EVENT_HANDLER ){
			return m_objects.event_handler;
		}
		return 0;
	}

	ui::Element * element() const {
		if( m_type & FLAG_IS_ELEMENT ){
			return m_objects.element;
		}
		return 0;
	}

	ui::ListItem * list_item() const {
		if( m_type & FLAG_IS_LIST_ITEM ){
			return m_objects.list_item;
		}
		return 0;
	}

	/*! \details Accesses the button associated with the event.
	 *
	 * @return A pointer to the button or zero if the event is not a button event.
	 *
	 */
	ev::Button * button() const {
		if( m_type & FLAG_IS_BUTTON ){
			return m_objects.button;
		}
		return 0;
	}

	/*! \details Accesses the signal associated with the event.
	 *
	 * @return A pointer to the signal if type is Event::SIGNAL or zero if not.
	 */
	sys::Signal * signal() const {
		if( m_type == SIGNAL ){
			return m_objects.signal;
		}
		return 0;
	}

	void * application() const {
		if( m_type == APPLICATION ){
			return m_objects.object;
		}
		return 0;
	}

	void set_event(
			enum event_type t,
			void * object = 0
			){
		m_type = t;
		m_objects.object = 0;
	}

	void set_event(
			enum event_type t,
			ev::Button * button
			){
		if( t & FLAG_IS_BUTTON ){
			m_type = t;
			m_objects.button = button;
		}
	}

	void set_type(enum event_type v) { m_type = v; }

	enum event_type type() const { return m_type; }

private:

	/*! \cond */
	enum event_type m_type;
	union event_objects {
		void * object;
		ui::Element * element;
		ev::Button * button;
		ui::ListItem * list_item;
		sys::Signal * signal;
		EventHandler * event_handler;
	} m_objects;
	/*! \endcond */


};

}

#endif /* SAPI_EV_EVENT_HPP_ */
