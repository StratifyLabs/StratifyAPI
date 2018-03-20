#include "ssm/State.hpp"

using namespace ssm;

State::State(){
    m_event_loop = 0;
}

State * State::handle_event(const Event & event){
    return this;
}

