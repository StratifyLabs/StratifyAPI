#ifndef SAPI_ARG_ARGUMENT_HPP
#define SAPI_ARG_ARGUMENT_HPP

#include <utility>
#include <pthread.h>
#include <signal.h>

#if !defined __link || defined __macosx
#include <unistd.h>
#include <sched.h>
#endif

#if defined __win32
typedef int pid_t;
#endif

#if defined __link
#include <sos/link.h>
#endif

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

#include "../api/ArgumentObject.hpp"

namespace var {
class ConstString;
class DataReference;
class JsonValue;
}

namespace fs {
class File;
}

namespace sys {
class Thread;
class Printer;
}

namespace inet {
class SocketAddress;
}

namespace chrono {
class Seconds;
class Milliseconds;
class Microseconds;
class Nanoseconds;
}

namespace arg {

template <typename T, typename Parameter>
class Argument {
public:
	explicit Argument(T const& value) : m_value(value) {}

	template<typename T_ = T>
	explicit Argument(T&& value,
							typename std::enable_if<!std::is_reference<T_>{},
							std::nullptr_t>::type = nullptr)
		: m_value(std::move(value)) {}

	T& argument() { return m_value; }
	T const& argument() const {return m_value; }
private:
	T m_value;
};


template <typename T, typename Parameter, typename A>
class ImplicitArgument {
public:
	ImplicitArgument(T const& value) : m_value(value) {}
	ImplicitArgument(A const& value) : m_value(value.argument()) {}

	template<typename T_ = T>
	ImplicitArgument(T&& value,
						  typename std::enable_if<!std::is_reference<T_>{},
						  std::nullptr_t>::type = nullptr)
		: m_value(std::move(value)) {}

	T& argument() { return m_value; }
	T const& argument() const {return m_value; }
private:
	T m_value;
};

}


#endif // SAPI_ARG_ARGUMENT_HPP
