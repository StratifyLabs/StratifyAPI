/* Copyright 2014-2016 Tyler Gilbert, Inc; All Rights Reserved
 *
 */

#include <cstdio>
#include "sys/requests.h"
#include "ui/EventLoop.hpp"
#include "sys.hpp"
#include "draw.hpp"

namespace ui {

EventLoop::EventLoop(ui::Element & start_element, sgfx::Bitmap & bitmap, sgfx::Bitmap * scratch) : sm::EventLoop(start_element) {
    //use the full screen by default
    m_drawing_attr.set_bitmap(bitmap);
    m_drawing_attr.set_scratch(scratch);
    m_drawing_attr.set_point(0,0);
    m_drawing_attr.set_dim(1000, 1000);
}


Element * EventLoop::handle_event(Element * current_element, const ui::Event & event, const DrawingAttr & drawing_attr, EventLoop * event_loop){
    Element * next_element = current_element;
    if( current_element ){
        if( event.type() != Event::NONE ){
            current_element->set_event_loop(event_loop);
            next_element = current_element->handle_event(event, drawing_attr);
        }

        if( next_element != current_element ){
            next_element->set_event_loop(event_loop);
            handle_transition(current_element, next_element, drawing_attr);
        }
    }
    return next_element;
}

void EventLoop::handle_transition(Element * current_element, Element * next_element, const draw::DrawingAttr & drawing_attr){
    //exit the current element
    if( current_element ){
        current_element->handle_event(Event(Event::EXIT, next_element), drawing_attr);
    }

    if( next_element ){
        if( drawing_attr.is_bitmap_available() ){
            Animation animation(next_element->animation_attr());
            if( (animation.type() != Animation::NONE) && drawing_attr.scratch() ){
                animation.init(0, next_element, drawing_attr);
                animation.exec();
            }
        }


        //enter the new element
        next_element->handle_event(Event(Event::ENTER), drawing_attr);
    }
}

bool EventLoop::handle_event(const Event & event){
    Element * tmp = current_element();
    tmp = handle_event(current_element(), event, drawing_attr(), this);
    if( tmp != current_element() ){
        set_current_element(tmp);
        return true;
    }
    return false;
}


void EventLoop::start(Element * element, const draw::DrawingAttr & drawing_attr){
    if( element ){
        element->handle_event(Event(Event::SETUP), drawing_attr);
        if( element->is_redraw_pending() ){
            drawing_attr.bitmap().refresh();
            element->set_redraw_pending(false);
        }
        handle_transition(0, element, drawing_attr);
    }
}

void EventLoop::start(){
    if( current_element() ){
        current_element()->set_event_loop(this);
    }
    start(current_element(), drawing_attr());
    update_timer().start();
}

void EventLoop::loop(){
    while( current_element() != 0 ){

        loop_timer().restart();

        if( m_drawing_attr.is_bitmap_available() ){
            m_drawing_attr.bitmap().wait(refresh_wait_resolution()); //wait until video memory is free to write
        }

        process_events(); //process all events (this will modify the video memory)
        check_loop_for_update(); //check for Event::UPDATE based on update_timer()

        if( current_element() ){ //update the display if the current element is still valid
            if( current_element()->is_redraw_pending() ){
                if( m_drawing_attr.is_bitmap_available() ){
                    m_drawing_attr.bitmap().refresh();
                }
                current_element()->set_redraw_pending(false);
            }
        }

        check_loop_for_hibernate();
    }
}



} /* namespace ui */
