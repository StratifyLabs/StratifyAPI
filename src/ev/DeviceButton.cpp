/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see LICENSE.md for rights.
/* Copyright 2016 tgil All Rights Reserved */

#include "ev/DeviceButton.hpp"

using namespace ev;

DeviceButton::DeviceButton(){
	memset(&m_status, 0, sizeof(m_status));
	m_location = 0;
}


int DeviceButton::init(){
	button_attr_t attr;
	memset(&attr, 0, sizeof(attr));
	attr.o_flags = BUTTON_FLAG_INITIALIZE;
	return ioctl(
				IoRequest(I_BUTTON_SETATTR),
				IoArgument(&attr)
				);
}


int DeviceButton::set_attributes(
		Location location,
		enum ev::Event::button_id id,
		HeldThreshold held_threshold,
		ActuatedThreshold actuated_threshold
		){
	button_attr_t attr;
	int ret;
	memset(&attr, 0, sizeof(attr));
	attr.o_flags = BUTTON_FLAG_SET_ID;
	attr.id = id;
	attr.loc = location.argument();

	if( held_threshold.argument().is_valid() ){
		attr.o_flags |= BUTTON_FLAG_SET_HELD_THRESHOLD;
		attr.threshold_msec = held_threshold.argument().milliseconds();
	}

	ret = ioctl(
				IoRequest(I_BUTTON_SETATTR),
				IoArgument(&attr)
				);

	if( (ret >= 0 ) &&
		 actuated_threshold.argument().is_valid() ){
		attr.o_flags = BUTTON_FLAG_SET_ACTUATED_THRESHOLD;
		attr.threshold_msec = actuated_threshold.argument().milliseconds();
		ret = ioctl(
					IoRequest(I_BUTTON_SETATTR),
					IoArgument(&attr)
					);
	}

	return ret;
}


chrono::MicroTime DeviceButton::get_duration(){
	chrono::MicroTime duration = chrono::Milliseconds(m_status.active_duration_msec);
	m_status.active_duration_msec = 0;
	return duration;
}

bool DeviceButton::get_pressed(){
	if( m_status.o_flags & BUTTON_FLAG_IS_PRESSED ){
		m_status.o_flags &= ~BUTTON_FLAG_IS_PRESSED;
		return true;
	}
	return false;
}

bool DeviceButton::get_released(){
	if( m_status.o_flags & BUTTON_FLAG_IS_RELEASED ){
		m_status.o_flags &= ~BUTTON_FLAG_IS_RELEASED;
		return true;
	}
	return false;
}

bool DeviceButton::get_actuated(){
	if( m_status.o_flags & BUTTON_FLAG_IS_ACTUATED ){
		m_status.o_flags &= ~BUTTON_FLAG_IS_ACTUATED;
		return true;
	}
	return false;
}

bool DeviceButton::get_held(){
	if( m_status.o_flags & BUTTON_FLAG_IS_HELD ){
		m_status.o_flags &= ~BUTTON_FLAG_IS_HELD;
		return true;
	}
	return false;
}

bool DeviceButton::is_active() const {
	return (m_status.o_flags & BUTTON_FLAG_IS_ACTIVE) != 0;
}

void DeviceButton::update(){
	u32 o_flags = m_status.o_flags;
	if( (seek(
			  m_location*sizeof(m_status)
			  ) >= 0) && (read(m_status) == sizeof(m_status)) ){
		//keep track of the flags because the driver will clear the flags before the EventLoop does
		m_status.o_flags |= o_flags;
	} else {
		Device::set_error_number(EIO);
		memset(&m_status, 0, sizeof(m_status));
	}
}

void DeviceButton::reset(){
	seek(0);
	while( read(m_status) == sizeof(m_status) ){
		;
	}
	memset(&m_status, 0, sizeof(m_status));
}
