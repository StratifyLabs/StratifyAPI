/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see LICENSE.md for rights.
#include "ev/EventHandler.hpp"

using namespace ev;

EventHandler::EventHandler(){
	m_event_loop = 0;
}

EventHandler * EventHandler::handle_event(const Event & event){
	return this;
}

