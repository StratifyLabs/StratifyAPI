/* Copyright 2016 tgil All Rights Reserved */

#include "ui/DeviceButton.hpp"

using namespace ui;

DeviceButton::DeviceButton(){
    memset(&m_status, 0, sizeof(m_status));
    m_location = 0;
}


int DeviceButton::init(){
    button_attr_t attr;
    memset(&attr, 0, sizeof(attr));
    attr.o_flags = BUTTON_FLAG_INITIALIZE;
    return ioctl(I_BUTTON_SETATTR, &attr);
}


int DeviceButton::set_attributes(int location, int id,
                                 const chrono::MicroTime held_threshold,
                                 const chrono::MicroTime actuated_threshold){
    button_attr_t attr;
    int ret;
    memset(&attr, 0, sizeof(attr));
    attr.o_flags = BUTTON_FLAG_SET_ID;
    attr.id = id;
    attr.loc = location;

    if( held_threshold.is_valid() ){
        attr.o_flags |= BUTTON_FLAG_SET_HELD_THRESHOLD;
        attr.threshold_msec = held_threshold.milliseconds();
    }

    ret = ioctl(I_BUTTON_SETATTR, &attr);

    if( (ret >= 0 ) && actuated_threshold.is_valid() ){
        attr.o_flags = BUTTON_FLAG_SET_ACTUATED_THRESHOLD;
        attr.threshold_msec = actuated_threshold.milliseconds();
        ret = ioctl(I_BUTTON_SETATTR, &attr);
    }

    return ret;
}


chrono::MicroTime DeviceButton::get_duration(){
    chrono::MicroTime duration = chrono::MicroTime::from_milliseconds(m_status.active_duration_msec);
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
    if( (seek(m_location*sizeof(m_status)) >= 0) && (read(&m_status, sizeof(m_status)) == sizeof(m_status)) ){
        //keep track of the flags because the driver will clear the flags before the EventLoop does
        m_status.o_flags |= o_flags;
    } else {
        set_error_number(EIO);
        memset(&m_status, 0, sizeof(m_status));
    }
}

void DeviceButton::reset(){
    seek(0);
    while( read(&m_status, sizeof(m_status)) == sizeof(m_status) ){
        ;
    }
    memset(&m_status, 0, sizeof(m_status));
}
