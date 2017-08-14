/*! \file */ //Copyright 2011-2016 Tyler Gilbert; All Rights Reserved

#ifndef SIGNAL_HPP_
#define SIGNAL_HPP_

#ifndef __link

#include <pthread.h>
#include <signal.h>
#include <sos/sos.h>

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
 * #include <sapi/sys.hpp>
 *
 * volatile bool wait_for_signal;
 * void my_handler(int a){
 * 	//do something
 * 	wait_for_signal = false;
 * }
 *
 * int main(int argc, char * argv[]){
 * 	SignalHandler handler(my_handler);
 * 	Signal event(SIGINT);
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
	/*! \details Constructs a signal handler.
	 *
	 * @param handler The function to execute with an associated signal
	 */
	SignalHandler(void (*handler)(int) ){
		m_sig_action.sa_handler = (_sig_func_ptr)handler;
		m_sig_action.sa_flags = 0;
		m_sig_action.sa_mask = 0;
	}

	/*! \details Constructs a sigaction handler.
	 *
	 * @param sigaction The action to execute with an associated signal
	 * @param flags Not used
	 * @param mask Not used
	 *
	 */
	SignalHandler(void (*sigaction)(int, siginfo_t*, void*), int flags = 0, sigset_t mask = 0){
		m_sig_action.sa_sigaction = sigaction;
		m_sig_action.sa_flags = flags | (1<<SA_SIGINFO);
		m_sig_action.sa_mask = mask;
	}

	/*! \details Accesses the sigaction member. */
	const struct sigaction * sigaction() const { return &m_sig_action; }

private:
	struct sigaction m_sig_action;
};

/*! \brief Class for sending Signal events (see SignalHandler for an example) */
class Signal {
public:

	enum {
		ABRT /*! Abort signal, default action is to abort */ = SIGABRT,
		ALRM /*! Alarm signal, default action is to terminate */ = SIGALRM,
		BUS /*! Bus signal, default action is to abort */ = SIGBUS,
		CHLD /*! Child signal, default action is to ignore */ = SIGCHLD,
		CONT /*! Continue signal, default action is to continue */ = SIGCONT,
		FPE /*! FPE signal, default action is to abort */ = SIGFPE,
		HUP /*! Hangup signal, default action is to terminate */ = SIGHUP,
		ILL /*! Illegal signal, default action is to abort */ = SIGILL,
		INT /*! Interrupt signal, default action is to terminate */ = SIGINT,
		KILL /*! Kill signal, default action is to terminate (cannot be caught or ignored) */ = SIGKILL,
		PIPE /*! Pipe signal, default action is to terminate */ = SIGPIPE,
		QUIT /*! Quit signal, default action is to abort */ = SIGQUIT,
		SEGV /*! Segmentation signal, default action is to abort */ = SIGSEGV,
		STOP /*! Stop signal, default action is to stop (cannot be caught or ignored) */ = SIGSTOP,
		TERM /*! Terminal signal, default action is to terminate */ = SIGTERM,
		TSTP /*! TSTP signal, default action is to stop */ = SIGTSTP,
		TTIN /*! TTIN signal, default action is to stop */ = SIGTTIN,
		TTOU /*! TTOU signal, default action is to stop */ = SIGTTOU,
		USR1 /*! User signal, default action is to terminate */ = SIGUSR1,
		USR2 /*! User signal, default action is to terminate */ = SIGUSR2,
		POLL /*! Poll signal, default action is to terminate */ = SIGPOLL,
		PROF /*! PROF signal, default action is to terminate */ = SIGPROF,
		SYS /*! System signal, default action is to abort */ = SIGSYS,
		TRAP /*! Trap signal, default action is to abort */ = SIGTRAP,
		URG /*! URG signal, default action is to ignore */ = SIGURG,
		TALRM /*! TALRM signal, default action is to terminate */ = SIGVTALRM,
		XCPU /*! XCPU signal, default action is to abort */ = SIGXCPU,
		XFSZ /*! XFSZ signal, default action is to abort */ = SIGXFSZ,
		RTMIN /*! Real time signal, default action is to ignore */ = SIGRTMIN,
		RT /*! Real time signal, default action is to ignore */ = SIGRTMIN + 1,
		RTMAX /*! Real time signal, default action is to ignore */ = SIGRTMAX,
	};

	/*! \details Constructs an event based on a signal number.
	 *
	 * @param signo The signal number
	 * @param sigvalue The signal value
	 *
	 */
	Signal(int signo, int sigvalue = 0){ m_signo = signo; m_sigvalue.sival_int = sigvalue; }

	/*! \details Returns a UI Event based on this signal event. */
	ui::Event event(){ return ui::Event(ui::Event::SIGNAL, this); }

	/*! \details Sends a signal to a process.
	 *
	 * @param pid The process ID of the receiving signal
	 * @return Zero on success
	 */
	int trigger(pid_t pid) const { return ::kill(pid, m_signo); }

	/*! \details Sends a signal and associated value to a process.
	 *
	 * @param pid  The process ID of the receiving signal
	 * @param value The value associated with the signal (user defined)
	 * @return Zero on success
	 */
	int trigger_value(pid_t pid) const {
		return ::sigqueue(pid, m_signo, m_sigvalue);
	}

	/*! \details Sends a signal to a thread within a process.
	 *
	 * @param t The thread ID
	 * @return Zero on success
	 */
	int trigger(pthread_t t) const { return ::pthread_kill(t, m_signo); }

	/*! \details Triggers the event on the current thread. */
	int trigger() const { return ::pthread_kill(pthread_self(), m_signo); }

	/*! \details Sets the event handler.
	 *
	 * @param handler A reference to the SignalHandler object
	 * @return Zero on success
	 */
	int set_handler(const SignalHandler & handler) const;

	/*! \details Returns the signal number. */
	int signo() const { return m_signo; }

	/*! \details Returns the signal value. */
	int sigvalue() const { return m_sigvalue.sival_int; }

	/*! \details Returns the signal pointer. */
	void * sigptr() const { return m_sigvalue.sival_ptr; }

private:
	int m_signo;
	union sigval m_sigvalue;

};

};

#endif

#endif /* SIGNAL_HPP_ */
