/* Copyright 2016 tgil All Rights Reserved */

#ifndef UI_EVENT_HPP_
#define UI_EVENT_HPP_

namespace sys {
class SignalEvent;
};

namespace ui {

class Button;

class Event {
public:

	enum {
		BUTTON_FLAG = 0x80
	};

	enum event_type {
		NONE = 0,
		SETUP /*! This event is called at startup after all object have been constructed */ = 1,
		ENTER /*! This event is called when the element becomes active */ = 2,
		UPDATE /*! This event is called in a loop while the element is active */ = 3,
		BUTTON_ACTUATION /*! This event is called when a button is actuated (pressed and released). Use button() to access button details. */ = BUTTON_FLAG | 4,
		BUTTON_HOLD /*! This event is called when a button is held. Use button() to access button details. */ = BUTTON_FLAG | 5,
		BUTTON_PRESSED /*! This event is called when a button is pressed. Use button() to access button details. */ = BUTTON_FLAG | 6,
		BUTTON_RELEASED /*! This event is called when a button is released. Use button() to access button details. */ = BUTTON_FLAG | 7,
		NETWORK_DATA /*! This event is called when data arrives on the network */ = 8,
		SIGNAL /*! This event is called when data arrives on the network */ = 9,
		APPLICATION /*! This event is an application specific where the data is specified by the application */ = 10,

		//EVENT_TYPE_TOTAL //omit TOTAL so that the compiler doesn't complain about not handling the case

	};

	/*! \details Element events */
	enum button_id {
		NO_BUTTON,
		UP_BUTTON,
		DOWN_BUTTON,
		LEFT_BUTTON,
		RIGHT_BUTTON,
		SELECT_BUTTON,
		BACK_BUTTON,
		EXIT_BUTTON,
		//EVENT_BUTTON_TOTAL //omit TOTAL so that the compiler doesn't complain about not handling the case
	};


	Event();

	/*! \details Construct a new event with the specified type and object */
	Event(enum event_type type, void * object = 0){
		m_type = type;
		m_objects.object = object;
	}

	Event(enum event_type type, ui::Button * button){
		m_type = type;
		m_objects.button = button;
	}


	ui::Button * button() const {
		if( m_type & BUTTON_FLAG ){
			return m_objects.button;
		}
		return 0;
	}

	sys::SignalEvent * signal() const {
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

	void set_event(enum event_type t, void * object = 0){
		m_type = t;
		m_objects.object = 0;
	}

	void set_event(enum event_type t, ui::Button * button){
		if( t & BUTTON_FLAG ){
			m_type = t;
			m_objects.button = button;
		}
	}

	void set_type(enum event_type v) { m_type = v; }

	enum event_type type() const { return m_type; }

private:

	enum event_type m_type;
	union event_objects {
		void * object;
		Button * button;
		sys::SignalEvent * signal;
	} m_objects;


};

};

#endif /* UI_EVENT_HPP_ */
