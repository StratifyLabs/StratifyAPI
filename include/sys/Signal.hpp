/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see LICENSE.md for rights.

#ifndef SAPI_SYS_SIGNAL_HPP_
#define SAPI_SYS_SIGNAL_HPP_

#include "../arg/Argument.hpp"
#include "../ev/Event.hpp"
#include "../api/SysObject.hpp"
#include "Sched.hpp"
#include "Thread.hpp"

#if defined __link
typedef void (*signal_function_callback_t)(int);
#if defined __win32
typedef void (*signal_action_callback_t)(int, void*, void*);
#define SIGNAL_SIGINFO_FLAG 0
#else
typedef void (*signal_action_callback_t)(int, siginfo_t*, void*);
typedef signal_function_callback_t _sig_func_ptr;
#define SIGNAL_SIGINFO_FLAG SA_SIGINFO
#endif
#endif


#if defined __StratifyOS__
typedef void (*signal_function_callback_t)(int);
typedef void (*signal_action_callback_t)(int, siginfo_t*, void*);
#define SIGNAL_SIGINFO_FLAG (1<<SA_SIGINFO)
#endif

#if defined __win32
#define sapi_signal_posix_kill raise
typedef u32 sigset_t;
struct sigaction {
	signal_function_callback_t sa_handler;
	signal_action_callback_t sa_sigaction;
	u32 sa_flags;
	u32 sa_mask;
};
union sigval {
	int sival_int;
	void * sival_ptr;
};
#else
#define sapi_signal_posix_kill kill
#endif


namespace sys {

class SignalFlags {
public:
	using Function = arg::Argument<signal_function_callback_t, struct SignalFlagsFunctionTag>;
	using ActionFunction = arg::Argument<signal_action_callback_t, struct SignalFlagsActionFunctionTag>;
	using Flags = arg::Argument<u32, struct SignalFlagsFlagsTag>;
	using Mask = arg::Argument<u32, struct SignalFlagsMaskTag>;

	using ProcessId = Sched::ProcessId;
	using ValueInteger = arg::Argument<int, struct SignalFlagsValueIntegerTag>;
	using ValuePointer = arg::Argument<void *, struct SignalFlagsValuePointerTag>;


	enum number {
		ABRT /*! Abort signal, default action is to abort */ = SIGABRT,
		FPE /*! FPE signal, default action is to abort */ = SIGFPE,
		INT /*! Interrupt signal, default action is to terminate */ = SIGINT,
		ILL /*! Illegal signal, default action is to abort */ = SIGILL,
		SEGV /*! Segmentation signal, default action is to abort */ = SIGSEGV,
		TERM /*! Terminal signal, default action is to terminate */ = SIGTERM,
#if !defined __win32
		ALRM /*! Alarm signal, default action is to terminate */ = SIGALRM,
		BUS /*! Bus signal, default action is to abort */ = SIGBUS,
		CHLD /*! Child signal, default action is to ignore */ = SIGCHLD,
		CONT /*! Continue signal, default action is to continue */ = SIGCONT,
		HUP /*! Hangup signal, default action is to terminate */ = SIGHUP,
		KILL /*! Kill signal, default action is to terminate (cannot be caught or ignored) */ = SIGKILL,
		PIPE /*! Pipe signal, default action is to terminate */ = SIGPIPE,
		QUIT /*! Quit signal, default action is to abort */ = SIGQUIT,
		STOP /*! Stop signal, default action is to stop (cannot be caught or ignored) */ = SIGSTOP,
		TSTP /*! TSTP signal, default action is to stop */ = SIGTSTP,
		TTIN /*! TTIN signal, default action is to stop */ = SIGTTIN,
		TTOU /*! TTOU signal, default action is to stop */ = SIGTTOU,
		USR1 /*! User signal, default action is to terminate */ = SIGUSR1,
		USR2 /*! User signal, default action is to terminate */ = SIGUSR2,
		PROF /*! PROF signal, default action is to terminate */ = SIGPROF,
		SYS /*! System signal, default action is to abort */ = SIGSYS,
		TRAP /*! Trap signal, default action is to abort */ = SIGTRAP,
		URG /*! URG signal, default action is to ignore */ = SIGURG,
		TALRM /*! TALRM signal, default action is to terminate */ = SIGVTALRM,
		XCPU /*! XCPU signal, default action is to abort */ = SIGXCPU,
		XFSZ /*! XFSZ signal, default action is to abort */ = SIGXFSZ,
#endif
#if !defined __link
		POLL /*! Poll signal, default action is to terminate */ = SIGPOLL,
		RTMIN /*! Real time signal, default action is to ignore */ = SIGRTMIN,
		RT /*! Real time signal, default action is to ignore */ = SIGRTMIN + 1,
		RTMAX /*! Real time signal, default action is to ignore */ = SIGRTMAX
#endif
	};

};

/*! \brief Signal Handler Class
 * \details This class is used for handling signal events.
 *
 * This class is a wrapper for the underlying POSIX signals API.
 *
 *
 *
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
 * 		Timer::wait_milliseconds(1);
 * 	}
 *
 * 	return 0;
 *
 * }
 *
 * \endcode
 */
class SignalHandler : public SignalFlags {
public:

	/*! \details Constructs a signal handler.
	 *
	 * @param handler The function to execute with an associated signal
	 */
	SignalHandler(
			Function signal_function
			){
#if defined __win32
		m_sig_action.sa_handler = signal_function.argument();
#else
		m_sig_action.sa_handler = (_sig_func_ptr)signal_function.argument();
#endif
		m_sig_action.sa_flags = 0;
		m_sig_action.sa_mask = 0;
	}

	/*! \details Constructs a sigaction handler.
	 *
	 * @param sigaction The action to execute with an associated signal
	 * @param flags Not used
	 * @param mask Not used
	 *
	 * \code
	 *
	 * void action_callback(int signo, siginfo_t * info, void * context);
	 *
	 * SignalHandler handler(action_callback);
	 *
	 * Signal alarm(Signal::INT, 10);
	 * alarm.set_handler(handler);
	 *
	 *
	 *
	 * \endcode
	 *
	 */
	SignalHandler(
			ActionFunction signal_action_function,
			Flags flags = Flags(0),
			Mask mask = Mask(0)
			){
		m_sig_action.sa_sigaction = signal_action_function.argument();
		m_sig_action.sa_flags = flags.argument() | SIGNAL_SIGINFO_FLAG;
		m_sig_action.sa_mask = mask.argument();
	}

	/*! \details Accesses the sigaction member. */
	const struct sigaction * sigaction() const { return &m_sig_action; }

private:
	struct sigaction m_sig_action;
};

/*! \brief Signal Class
 * \details This class is for sending POSIX style signals between
 * processes and threads.
 *
 * If you are brand new to Signals you can read the basics on [wikipedia](https://en.wikipedia.org/wiki/Signal_(IPC)).
 *
 * Basically, a signal can be sent from one process to another. Within
 * a process a signal can be sent to a thread.
 *
 * The operating system will pre-empt the receiving process to deliver
 * the signal. If the receiving process has registerd a signal
 * handler (see SignalHandler), the signal handler will be executed. Otherwise,
 * the default handler will be executed.
 *
 * The signal contains a signal number and an optional signal value.
 *
 * The send() methods are used to send just the signal number. The
 * queue() method sends the signal number as well as the signal value.
 *
 * The SignalHandler class allows the receiving thread to create
 * a handler for received signals. It can use a simple handler which
 * will just look at the signal number or it can use a more sophisticated
 * handler that will accept the signal number and signal value.
 *
 *
 * This code shows how to send a signal to another process.
 *
 * \code
 * int sigvalue = 100;
 * Signal sig(Signal::USR1, sigvalue);
 *
 * pid_t another_process_pid;
 *
 * sig.send(another_process_pid); //just sends USR1
 * sig.queue(another_process_pid); //send USR1 and sigvalue
 *
 * \endcode
 *
 * The receiving process and handle the signals like this:
 *
 * \code
 *
 * void signal_only_handler(int a){
 *   //handles just the signal number a = Signal::USR1
 * }
 *
 * void signal_action_handler(int a, siginfo_t * info, void * context){
 *  //a = Signal::USR1 info->si_value.sival_int = 100, context is always NULL
 * }
 *
 * SignalHandler my_signal_action_handler(signal_action_handler);
 *
 * //this will tell the process to register signal_action_handler() to handle USR1
 * Signal incoming_signal(Signal::USR1);
 * incoming_signal.set_handler(my_handler);
 *
 * \endcode
 *
 * This class handles Signal's created by sofware. The hal::DeviceSignal
 * class can be used to have the OS send a signal to a thread or process
 * when a hardware event happens (such as a when a GPIO interrupt happens).
 *
 */
class Signal : public api::WorkObject, public SignalFlags {
public:

	/*! \details Constructs an event based on a signal number.
	 *
	 * @param signo The signal number
	 * @param sigvalue The signal value
	 *
	 */
	Signal(
			enum number signo,
			ValueInteger signal_value = ValueInteger(0)
			){
		m_signo = signo;
		m_sigvalue.sival_int = signal_value.argument();
	}

	/*! \details Constructs an event based on a signal number.
	  *
	  * @param signo The signal number
	  * @param sigptr The signal value as a pointer
	  *
	  */
	Signal(
			enum number signo,
			void * signal_pointer
			){
		m_signo = signo;
		m_sigvalue.sival_ptr = signal_pointer;
	}

	/*! \details Returns a UI Event based on this signal event. */
	ev::Event event(){
		return ev::Event(ev::Event::SIGNAL, this);
	}

	/*! \details Sends a signal to a process.
	 *
	 * @param pid The process ID of the receiving signal
	 * @return Zero on success
	  *
	  * This method sends this signal to the specified PID.
	  * It uses the POSIX kill() function.
	 */
	int send(
			Sched::ProcessId pid
			) const {
		return ::sapi_signal_posix_kill(
			#if !defined __win32
					pid.argument(),
			#endif
					m_signo);
	}

#if !defined __link
	/*! \details Sends a signal and associated sigvalue to a process.
	 *
	  * @param pid The process ID of the receiving signal
	 * @return Zero on success
	  *
	  * This method sends this signal to the specified PID.
	  * It uses the POSIX sigqueue() function meaning the the signal
	  * will be sent along with signo and sigvalue.
	  *
	 */
	int queue(
			pid_t pid
			) const {
		return ::sigqueue(pid, m_signo, m_sigvalue);
	}

	int queue(
			ProcessId pid
			) const {
		return queue(pid.argument());
	}
#endif

	/*! \details Sends a signal to a thread within a process.
	 *
	 * @param t The thread ID
	 * @return Zero on success
	 */
	int send(
			pthread_t t
			) const {
		return ::pthread_kill(t, m_signo);
	}

	/*! \details Triggers the event on the current thread. */
	int send() const {
		return ::pthread_kill(pthread_self(), m_signo);
	}

	/*! \details Sets the event handler.
	 *
	 * @param handler A reference to the SignalHandler object
	 * @return Zero on success
	 */
	int set_handler(
			const SignalHandler & handler
			) const;


	/*! \details Assigns the handler to the event.
	 *
	 * @param handler A reference to the SignalHandler
	 *
	 * \code
	 *	void handle_abort(int a){
	 *
	 * }
	 *
	 * Signal abort_signal(Signal::ABRT);
	 * abort_signal << SignalHandler(handle_abort);
	 *
	 * //code is all set up to execute handle_abort() when SIGABRT fires
	 * abort_signal.send(); //send abort to self
	 *
	 * \endcode
	 *
	 *
	 */
	Signal & operator << (const SignalHandler & handler){
		set_handler(handler);
		return *this;
	}

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

}


#endif /* SAPI_SYS_SIGNAL_HPP_ */
