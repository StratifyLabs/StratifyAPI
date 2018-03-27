#include "ev/EventHandler.hpp"

using namespace ev;

EventHandler::EventHandler(){
    m_event_loop = 0;
}

EventHandler * EventHandler::handle_event(const Event & event){
    return this;
}

