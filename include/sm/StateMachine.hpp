#ifndef STATEMACHINE_HPP
#define STATEMACHINE_HPP

#include <mcu/types.h>

#include "../var/Data.hpp"
#include "../sys/Timer.hpp"

namespace sm {

class State;

class Condition {
public:

    virtual const char * name() = 0;
    virtual bool is_true() const = 0;

    State * target_state(){
        return m_target_state;
    }

protected:
    void set_state(State * target_state){
        m_target_state = target_state;
    }

private:
    State * m_target_state;

};

class TimerCondition : public Condition {
public:

    TimerCondition(sys::Timer & timer) : m_timer(timer){}

    bool is_true() const {
        return m_timer.calc_value() >= timeout();
    }

    void set_timeout(const sys::MicroTime & value){
        m_timeout = value;
    }

    const sys::MicroTime & timeout() const {
        return m_timeout;
    }

private:
    sys::Timer & m_timer;
    sys::MicroTime m_timeout;
};

class Action {
public:
    virtual const char * name() const = 0;

private:

};


class State {
public:
    virtual const char * name() const = 0;


private:

    //pointer list to entry actions
    //pointer list to conditions

};

/*! \brief State Machine Class
 * \details This class implements a simple state machine. The state
 * machine will execute a method based on the state.
 *
 *
 */
template<class container_class, typename return_type, typename...args> class StateMachine {
public:

    /*! \details Defines the method type to execute with each state. */
    typedef return_type (container_class::*state_method_t)(args...);

    StateMachine(u32 count = 0){
        m_table_count = 0;
        m_state = 0;
        if( count ){
            if( m_state_table.alloc(count * sizeof(state_method_t)) < 0 ){
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
        state_method_t * table = (state_method_t *)m_state_table.data();
        return table;
    }

    var::Data m_state_table;
    u32 m_table_count;

};

}

#endif // STATEMACHINE_HPP
