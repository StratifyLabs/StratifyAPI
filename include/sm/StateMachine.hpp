/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see LICENSE.md for rights.
#ifndef SAPI_SM_STATEMACHINE_HPP_
#define SAPI_SM_STATEMACHINE_HPP_

#include "../api/SmObject.hpp"
#include "../var/Data.hpp"
#include "../sys/Timer.hpp"
#include "../var/ConstString.hpp"

/*! \cond */

namespace sm {

class State;
class StateMachine;
class Condition;
class Action;

/*! \brief State Machine Object
 *
 */
class Object : public api::SmWorkObject {
public:
	//when an object is contructed -- add the item to a list
	Object(StateMachine & state_machine);

	virtual const char * name() = 0;
	typedef enum {
		TYPE_OBJECT,
		TYPE_CONDITION,
		TYPE_STATE,
		TYPE_ACTION,
		TYPE_MACHINE
	} type_t;

	virtual const type_t type() const { return TYPE_OBJECT; }
	StateMachine & state_machine(){ return m_state_machine; }

private:
	StateMachine & m_state_machine; // a reference to the owning machine
};

/*! \brief State Machine Condition
 *
 */
class Condition : public Object {
public:

	Condition(StateMachine & state_machine) : Object(state_machine){}
	virtual bool is_true() const = 0;
	const type_t type() const { return TYPE_CONDITION; }

protected:

private:

};

/*! \brief State Machine Action
 *
 */
class Action : public Object {
public:
	Action(StateMachine & state_machine) : Object(state_machine){}
	const type_t type() const { return TYPE_ACTION; }
	virtual void execute() = 0;

private:

};


/*! \brief State Machine State
 *
 */
class State : public Object {
public:

	State(StateMachine & state_machine) : Object(state_machine){}

	const type_t type() const { return TYPE_STATE; }

	/*! \details Adds an exit condition that determines
	  * when to transition to another state.
	  *
	  *
	  */
	void add_exit_condition(const Condition & condition, const State & state, const Action & action);

	/*! \details Adds a action that is executed every time
	  * the state is entered.
	  *
	  * @param action A reference to the action to add.
	  *
	  */
	void add_entry_action(const Action & action);

	/*! \details Adds a action that is executed every time
	  * the state is exited.
	  *
	  * @param action A reference to the action to add.
	  *
	  */
	void add_exit_action(const Action & action);

	/*! \details Adds an action that is executed
	  * periodically while the state is active.
	  *
	  * @param action A reference to the action to add.
	  *
	  */
	void add_action(const Action & action);

private:

	friend class StateMachine;
	void execute_action();
	void execute_entry_action();
	State * check_exit_condition();

	//list of entry actions
	//list of actions
	//list of exit conditions, exit actions, and exit states

};

class NoAction: public Action {
public:
	NoAction(StateMachine & state_machine) : Action(state_machine){}

	void execute(){
		//does nothing
	}
};


/*! \brief State Machine
 *
 */
class StateMachine : public Object {
public:

	StateMachine();

	virtual void execute_action();

	void execute();

	//set the active state by name
	void set_state(const char * name);

	//set the active state by reference value
	void set_state(const State * state);

	void generate_dot_code();

	//search for a state by name
	State * get_state(const char * name);

	void append_state(Object & state);
	void append_condition(Object & condition);
	void append_action(Object & action);

private:
	State * active_state();
	State * m_active_state;

	//list of conditions -- need to create var::List
	//list of actions
	//list of states

};


/*! \brief Simple State Machine Class
 * \details This class implements a simple state machine. The state
 * machine will execute a method based on the state.
 *
 *
 */
template<class container_class, typename return_type, typename...args> class SimpleStateMachine : public api::SmWorkObject {
public:

	/*! \details Defines the method type to execute with each state. */
	typedef return_type (container_class::*state_method_t)(args...);

	SimpleStateMachine(u32 count = 0){
		m_table_count = 0;
		m_state = 0;
		if( count ){
			if( m_state_table.allocate(count * sizeof(state_method_t)) < 0 ){
				//failed to alloc table
			} else {
				m_table_count = count;
				m_state_table.clear(); //set to all null pointers
			}
		}
	}

	void resize(u32 count){
		u32 current_count = m_table_count;
		u32 i;
		if( m_state_table.resize(count * sizeof(state_method_t)) >= 0 ){
			//resize was successful
			m_table_count = count;

			//if there are additional entries -- zero them out
			for(i=current_count; i < this->count(); i++){
				get_table()[i] = 0;
			}
		}
	}

	/*! \details Returns true if the current state is valid. */
	bool is_state_valid() const { return m_state < count(); }

	/*! \details Sets the method associated with the state. */
	void set_state_method(u32 state, state_method_t method){
		if( state < count() ){
			get_table()[state] = method;
		}
	}

	/*! \details Updates the state machine.
	  *
	  * This class is designed to be used within another
	  * loop. This method should be called periodically.
	  *
	  */
	return_type update(args... arguments){
		state_method_t method = get_state_update_method(state());
		if( method != 0 ){
			return (((container_class*)this)->*method)(arguments...);
		}
	}

	/*! \details Returns the currently active state.
	  *
	  */
	u32 state() const { return m_state; }

	void set_state(u32 state){ m_state = state; }

	/*! \details Returns the current error value.
	  */
	int error() const { return m_error; }

	/*! \details Returns the total number of states currently
	  * registered with the state machine.
	  *
	  */
	u32 count() const { return m_table_count; }

private:
	u32 m_state;
	int m_error;
	bool m_is_update_table_mutable;

	state_method_t get_state_update_method(u32 state){
		state_method_t method = 0;

		if( state < count() ){
			method = get_table()[state];
		}

		return method;
	}

	state_method_t * get_table() const {
		state_method_t * table = m_state_table.to<state_method_t>();
		return table;
	}

	var::Data m_state_table;
	u32 m_table_count;

};

}

/*! \endcond */

#endif // SAPI_SM_STATEMACHINE_HPP_
