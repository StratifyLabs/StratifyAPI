/*! \file */ //Copyright 2011-2016 Tyler Gilbert; All Rights Reserved

#ifndef SIGNAL_HPP_
#define SIGNAL_HPP_

#ifndef __link

#include <pthread.h>
#include <signal.h>
#include <stratify/stratify.h>
#include <mcu/mcu.h>

#include "../ui/Event.hpp"

namespace sys {

/*! \brief Signal Handler Class
 * \details This class is used for handling signal events.
 *
 * Here is an example of setting up a signal handler.
 *
 * \code
 *
 * #include <stfy/Sys.hpp>
 *
 * volatile bool wait_for_signal;
 * void my_handler(int a){
 * 	//do something
 * 	wait_for_signal = false;
 * }
 *
 * int main(int argc, char * argv[]){
 * 	SignalHandler handler(my_handler);
 * 	SignalEvent event(SIGINT);
 *
 * 	wait_for_signal = true;
 *
 * 	//this will cause my_handler() to be executed when SIGINT is received
 * 	event.set_handler(handler);
 *
 * 	//the program will stay in this loop until SIGINT is received
 * 	while( wait_for_signal == true ){
 * 		Timer::wait_msec(1);
 * 	}
 *
 * 	return 0;
 *
 * }
 *
 * \endcode
 */
class SignalHandler {
public:
	/*! \details Construct a signal handler
	 *
	 *
	 */
	SignalHandler(void (*handler)(int) ){
		m_sig_action.sa_handler = (_sig_func_ptr)handler;
		m_sig_action.sa_flags = 0;
		m_sig_action.sa_mask = 0;
	}

	/*! \details Construct a sigaction handler */
	SignalHandler(void (*sigaction)(int, siginfo_t*, void*), int flags = 0, sigset_t mask = 0){
		m_sig_action.sa_sigaction = sigaction;
		m_sig_action.sa_flags = flags | (1<<SA_SIGINFO);
		m_sig_action.sa_mask = mask;
	}

	/*! \details Access the sigaction member */
	const struct sigaction * sigaction() const { return &m_sig_action; }

private:
	struct sigaction m_sig_action;
};

/*! \brief Class for sending Signal events (see SignalHandler for an example) */
class SignalEvent {
public:

	/*! \details Construct an event based on a signal number */
	SignalEvent(int signo, int sigvalue = 0){ m_signo = signo; m_sigvalue.sival_int = sigvalue; }

	/*! \details Create a UI Event from this Signal event */
	ui::Event event(){ return ui::Event(ui::Event::SIGNAL, this); }

	/*! \details This method sends a signal to a process
	 *
	 * @param pid  The process ID of the receiving signal
	 * @return Zero on success
	 */
	int trigger(pid_t pid) const { return ::kill(pid, m_signo); }

	/*! \brief Send a signal to a process with a value associated with the signal */
	/*! \details This method sends a signal and associated value to a process.
	 *
	 * @param pid  The process ID of the receiving signal
	 * @param value The value associated with the signal (user defined)
	 * @return Zero on success
	 */
	int trigger_value(pid_t pid) const {
		return ::sigqueue(pid, m_signo, m_sigvalue);
	}

	/*! \details This method sends a signal to a thread within a process.
	 *
	 * @param t The thread ID
	 * @return Zero on success
	 */
	int trigger(pthread_t t) const { return ::pthread_kill(t, m_signo); }

	/*! \details Trigger the event on the current thread */
	int trigger() const { return ::pthread_kill(pthread_self(), m_signo); }

	/*! \details Set the event handler */
	int set_handler(const SignalHandler & handler) const;

	int signo() const { return m_signo; }
	int sigvalue() const { return m_sigvalue.sival_int; }
	void * sigptr() const { return m_sigvalue.sival_ptr; }

private:
	int m_signo;
	union sigval m_sigvalue;

};

/*! \brief Class for bridging physical events with Signal events */
/*! \details This class allows you to configure physical events
 * to trigger signals.  For example, a rising edge on a pin
 * can cause the system to send a signal to a thread.  Here is an example:
 * \code
 *
 * #include <stfy/sys.hpp>
 * #include <stfy/hal.hpp>
 *
 * volatile bool my_var;
 * void my_handler(int a){
 * 	//this executes on the rising edge of EINT0
 * 	my_var = true;
 * 	//you shouldn't use any non re-entrant functions here (such as printf() and malloc())
 *
 * }
 *
 * int main(int argc, char * argv[]){
 * 	Eint eint(0);
 *
 * 	SignalEventPhy phy_event(true, SIGUSR1);  //send SIGUSR1 to this thread on every rising edge
 * 	SignalHandler handler(my_handler);
 *
 * 	eint.init(0);
 * 	phy_event.set_handler(handler);
 *
 * 	//this configures the hardware to send the signal to this thread (my_handler() will execute)
 * 	//phy_event MUST exist as long as this action is set
 * 	eint.set_action( phy_event.action(EINT_ACTION_EVENT_RISING) );
 *
 * 	while(my_var == false){
 * 		Timer::wait_sec(1);
 * 	}
 *
 * 	return 0;
 * }
 *
 *
 * \endcode
 */
class SignalEventDev: public SignalEvent {
public:

	/*! \details This constructs a physical event.
	 *
	 * @param persistant If false, the signal will be sent only on the first hardware event
	 * @param signo The signal number
	 * @param sigcode The signal code
	 * @param sigvalue The signal value
	 */
	SignalEventDev(bool persistant, int signo, int sigcode = 0, int sigvalue = 0) : SignalEvent(signo){
		m_context.tid = pthread_self();
		m_context.si_sigcode = sigcode;
		m_context.si_signo = signo;
		m_context.keep = persistant;
		m_context.sig_value = sigvalue;
	}


	/*! \details Construct a physical event using a signal_callback_t data structure.
	 *
	 * @param context A copy of the signal_callback_t data to use to handle the event.
	 */
	SignalEventDev(signal_callback_t context) : SignalEvent(context.si_signo){
		this->m_context = context;
	}

	/*! \details This returns a mcu_action_t data structure that can
	 * be used to set the action associated with a hardware event.
	 *
	 * @param event The hardware event
	 * @param channel The hardware channel
	 * @return a copy of a mcu_action_t data structure
	 *
	 */
	mcu_action_t action(int event, int channel = 0){
		mcu_action_t a;
		a.callback = signal_callback;
		a.context = &m_context;
		a.channel = channel;
		a.event = event;
		return a;
	}

private:
	signal_callback_t m_context;
};

};

#endif

#endif /* SIGNAL_HPP_ */
