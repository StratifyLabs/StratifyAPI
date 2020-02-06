/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see LICENSE.md for rights.
#include "sm/StateMachine.hpp"

using namespace sm;


Object::Object(StateMachine & state_machine) : m_state_machine(state_machine){

	//the object needs to be added to the state machines lists
	switch(type()){
		case TYPE_STATE: m_state_machine.append_state(*this); break;
		case TYPE_CONDITION: m_state_machine.append_condition(*this); break;
		case TYPE_ACTION: m_state_machine.append_action(*this); break;
		case TYPE_OBJECT:
		case TYPE_MACHINE:
			break;
	}



}

StateMachine::StateMachine() : Object(*this) {

}

void StateMachine::set_state(const char * name){

}

void StateMachine::set_state(const State * state){
	//make sure the state is in the list of states

}

void StateMachine::generate_dot_code(){
	//go over the state list and show transitions

}

void StateMachine::execute(){
	State * next_state;

	if( active_state() ){
		active_state()->execute_entry_action();
	}

	do {

		execute_action(); //execute machine level actions

		//execute the active state actions
		active_state()->execute_action();

		//check for exit conditions to transition to new states
		next_state = active_state()->check_exit_condition();
		if( next_state != active_state() ){
			//debug output for a transition
			set_state(next_state);
		}

	} while( active_state() );
}


void State::execute_action(){
	//execute the actions in the list
}

void State::execute_entry_action(){
	//execute actions in entry list
}

State * State::check_exit_condition(){

	//check for exit conditions and set appropriate actions
	return this;
}
