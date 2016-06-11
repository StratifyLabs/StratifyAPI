/* Copyright 2016 tgil All Rights Reserved */

#ifndef UI_EVENT_HPP_
#define UI_EVENT_HPP_

namespace ui {

class Button;

class Event {
public:
	Event();

	enum {
		BUTTON_FLAG = 0x80
	};

	enum event_type {
		NONE = 0,
		SETUP = 1,
		ENTER = 2,
		UPDATE = 3,
		BUTTON_ACTUATION = BUTTON_FLAG | 4,
		BUTTON_HOLD = BUTTON_FLAG | 5,
		BUTTON_PRESSED = BUTTON_FLAG | 6,
		BUTTON_RELEASED = BUTTON_FLAG | 7,
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
	} m_objects;


};

};

#endif /* UI_EVENT_HPP_ */
