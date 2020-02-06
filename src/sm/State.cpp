/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see LICENSE.md for rights.
#include "ssm/State.hpp"

using namespace ssm;

State::State(){
    m_event_loop = 0;
}

State * State::handle_event(const Event & event){
    return this;
}

