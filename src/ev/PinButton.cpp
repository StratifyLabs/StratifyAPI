/* Copyright 2016 tgil All Rights Reserved */

#include "ev/PinButton.hpp"
using namespace ev;
using namespace sys;
using namespace hal;

chrono::MicroTime PinButton::m_held_duration = 800000;


PinButton::PinButton(int port, int pin, bool active_value) : Pin(port, pin) {
    m_event_id = ev::Event::NO_BUTTON;
	m_active_value = active_value;
}

bool PinButton::get_is_active() const {
	return Pin::get_value() == m_active_value;
}


void PinButton::reset(){
    m_flags.press_reported = false;
    m_flags.release_reported = true;
    m_flags.held_reported = false;
    m_timer.reset();
}

void PinButton::update(void){
    if( get_is_active() == true ){ //check to see if Pin is in the active state
        if( m_timer.is_running() == false ){
            m_flags.press_reported = 0;
            m_flags.release_reported = 0;
            m_flags.held_reported = 0;
            m_flags.actuation_reported = 0;
            m_flags.duration_reported = 0;
            m_timer.restart(); //start the timer if it hasn't been started
        }
    } else {
        if( m_timer.is_running() == true ){
            m_timer.stop(); //stop the timer (if it hasn't been stopped yet)
        }
    }
}

chrono::MicroTime PinButton::get_duration(){
    chrono::MicroTime value = m_timer.microseconds();

    //reset the timer so the duration is only returned once after the button is released
    if( m_timer.is_stopped() ){
        if( m_flags.duration_reported == 0 ){
            m_flags.duration_reported = 1;
        } else {
            value = 0;
        }
        //m_timer.reset();
    }

    return value;
}

bool PinButton::get_pressed(){
    if( m_flags.press_reported == 0 ){
        if( m_timer.is_running() ){
            m_flags.press_reported = 1;
            return true;
        }
    }
    return false;
}

bool PinButton::get_released(){
    if( m_flags.release_reported == 0 ){
        if( !m_timer.is_running() ){
            m_flags.release_reported = 1;
            return true;
        }
    }
    return false;
}

bool PinButton::get_actuated(){
    bool ret;
    u32 msec;

    ret = false;
    if( m_flags.actuation_reported == 0 ){
		msec = m_timer.milliseconds();
        //true if time meets minimum and less than maximum and timer is not running
		if( (msec > 5) && (msec < m_held_duration.milliseconds()) && (!m_timer.is_running()) ){
            ret = true;
            m_flags.actuation_reported = 1;
        }
    }

    return ret;
}

bool PinButton::get_held(){
    bool ret = false;

    if( m_flags.held_reported == 0 ){
        //true if timer is greater than a maximum and held has not been reported
		if ( (m_timer.milliseconds() >= m_held_duration.milliseconds()) && (m_timer.is_running()) ){
            ret = true;
            m_flags.held_reported = 1;
        }
    }
    return ret;
}
